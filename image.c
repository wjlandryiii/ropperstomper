#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "image.h"

image_t *load_image(char *filename){
	FILE *f;
	image_t *image;
	int bytes_read;

	image = malloc(sizeof(image_t));
	assert(image != NULL);
	image->sections.head = NULL;

	f = fopen(filename, "rb");
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	image->size = ftell(f);
	fseek(f, 0, SEEK_SET);
	//clearerr(f);
	fclose(f);
	f = fopen(filename, "rb");
	printf("position: %d\n", ftell(f));

	printf("file: %s\n", filename);
	printf("file size: %d\n", image->size);

	image->contents = malloc(image->size);
	assert(image->contents != NULL);


	bytes_read = 0;
	while(bytes_read < image->size){
		//printf("fread( %08x, 1, %08x, f);\n", (int)&(image->contents[bytes_read]),  (image->size - bytes_read) >= 512 ? 512 : (image->size - bytes_read));
		bytes_read += fread(&(image->contents[bytes_read]), 1, (image->size - bytes_read) >= 512 ? 512 : (image->size - bytes_read), f);
		if(feof(f)){
			//printf("EOF %d\n", bytes_read);
			break;
		}
		//printf("read bytes: %d\n", bytes_read);
	}
	fclose(f);

	if(image->contents[0] == 'M' && image->contents[1] == 'Z'){
		//ms-dos magic.  likely a windows pe.
		pe_parse(image);
	} else {
		printf("didn't pass magic test\n");
		printf("%c %c\n", image->contents[0], image->contents[1]);
		return NULL;
	}
	return image;
}

pe_image_msdos_header_t *pe_find_msdos_header(image_t *image);
pe_image_file_header_t *pe_find_file_header(image_t *image, pe_image_msdos_header_t *msdos_header);
pe_image_optional_header_t *pe_find_optional_header(image_t *image, pe_image_file_header_t *file_header);
pe_image_section_header_t *pe_find_first_section_header(image_t *image, pe_image_file_header_t *file_header);

int pe_parse(image_t *image){
	pe_image_msdos_header_t *msdos_header;
	pe_image_file_header_t *file_header;
	pe_image_optional_header_t *optional_header;
	pe_image_section_header_t *first_section_header;
	int i;
	ll_node_t **np;
	section_t *section;

	msdos_header = pe_find_msdos_header(image);
	assert(msdos_header != NULL);

	file_header = pe_find_file_header(image, msdos_header);
	assert(file_header != NULL);

	if(file_header->Machine != 0x14c)
		return -1;
	else
		printf("Machine: i386\n");
	image->arch = ARCH_x86;

	/*if(file_header->SizeOfOptionalHeader != sizeof(pe_image_optional_header_t)){
		printf("Unknown optional header size %04x != %04x\n", file_header->SizeOfOptionalHeader, sizeof(pe_image_optional_header_t));
		return -1;
	}*/

	optional_header = pe_find_optional_header(image, file_header);
	assert(optional_header != NULL);

	printf("Entry Point: %08x\n", optional_header->AddressOfEntryPoint);
	printf("Base of code: %08x\n", optional_header->BaseOfCode);

	first_section_header = pe_find_first_section_header(image, file_header);
	assert(first_section_header != NULL);

	np = &image->sections.head;
	for(i = 0; i < file_header->NumberOfSections; i++){
		*np = malloc(sizeof(ll_node_t));
		assert(*np != NULL);
		(*np)->next = NULL;
		(*np)->data = section = malloc(sizeof(section_t));
		section->name = malloc(9);
		assert(section->name != NULL);

		assert(image->size >= first_section_header[i].PointerToRawData + first_section_header[i].SizeOfRawData);

		memset(section->name, 0, 9);
		memcpy(section->name, first_section_header[i].Name, 8);
		section->start = image->contents + first_section_header[i].PointerToRawData;
		section->size = first_section_header[i].SizeOfRawData;
		section->executable = first_section_header[i].Characteristics & 0x20 ? 1 : 0;
		section->virtual_offset = first_section_header[i].VirtualAddress;
		section->virtual_size = first_section_header[i].Misc.VirtualSize;

		np = &(*np)->next;
	}
	return 0;
}

pe_image_msdos_header_t *pe_find_msdos_header(image_t *image){
	pe_image_msdos_header_t *msdos_header;

	if(image->size < sizeof(pe_image_msdos_header_t))
		return NULL;

	msdos_header = (pe_image_msdos_header_t *)image->contents;

	if(msdos_header->e_magic != 0x5A4D)
		return NULL;
	return msdos_header;
}

pe_image_file_header_t *pe_find_file_header(image_t *image, pe_image_msdos_header_t *msdos_header){
	int pe_signature;

	if(image->size < msdos_header->e_lfanew + 4 + sizeof(pe_image_file_header_t))
		return NULL;

	pe_signature = *(int *)&(image->contents[msdos_header->e_lfanew]);
	if(pe_signature != 0x00004550)
		return NULL;

	return (pe_image_file_header_t *)&(image->contents[msdos_header->e_lfanew + 4]);
}

pe_image_optional_header_t *pe_find_optional_header(image_t *image, pe_image_file_header_t *file_header){
	pe_image_optional_header_t *optional_header;

	if(image->size < (((char *)file_header) - image->contents) + sizeof(file_header) + sizeof(optional_header))
		return NULL;

	optional_header = (pe_image_optional_header_t *)(file_header+1);
	if(optional_header->Magic != 0x10b)
		return NULL;
	return optional_header;
}

pe_image_section_header_t *pe_find_first_section_header(image_t *image, pe_image_file_header_t *file_header){
	pe_image_section_header_t *first_section_header;

	first_section_header = (pe_image_section_header_t *)(((char *)(file_header+1)) + file_header->SizeOfOptionalHeader);

	if(image->size < ((char *)(first_section_header+1)) - image->contents)
		return NULL;

	return first_section_header;
}
