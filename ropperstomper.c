#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include "instructions_x86.h"
#include "gadgets.h"
#include "bintree.h"
#include "image.h"
#include "llist.h"
#include "gadget_search.h"

void loadshit(char *lib, char *seg, char **addr, DWORD *len);
//void find_rets(char *buff, int len);
//int inst_search_backwards(char *buff, int len, int pos);
//int found_gadget(char *buff, int len, int pos);
int print_decode_gadget(gadget_t *g);
void print_all_gadgets_btnode(bt_node_t *n);
void arrayize_gadgets(gadget_t **gadget_list, int *c, bt_node_t *n);
void add_gadget(char *buff, int len, int pos, int glength);
int count_cmp(void *gadgeta, void *gadgetb);
int inst_cmp(void *a, void *b);
void print_gadgets(gadgets_t *gadgets);

int main(int argc, char *argv[]){
	image_t *image;
	gadgets_t *gadgets;

	image = load_image(argv[1]);
	assert(image != NULL);

	gadgets = find_gadgets(image);
	assert(gadgets != NULL);


	print_gadgets(gadgets);

	printf("Done!\n");
	return 0;
}

/*

int mainB(int argc, char *argv[]){
	image_t *image;
	ll_node_t *snode;
	section_t *section;

	printf("argc: %d\n", argc);
	if(argc <= 1){
		printf("%s: [target.exe|target.dll]\n", argv[0]);
		return 0;
	}

	image = load_image(argv[1]);
	//image = load_image("C:\\Windows\\SysWOW64\\ntdll.dll");
	assert(image != NULL);

	snode = image->sections.head;
	while(snode != NULL){
		section = snode->data;
		printf("%-8s %08x %08x %08x %08x %c\n", section->name, section->start, section->size, section->virtual_offset, section->virtual_size, section->executable ? 'X' : '-');
		snode = snode->next;
		if(section->executable != 0){
			find_rets(section->start, section->size);
		}
	}
	print_gadgets();
	// print_all_gadgets_btnode(gadgets.root);
	return 0;
}

int mainA(int argc, char *argv[]){
	char *addr;
	gadget_t **gadget_list;
	DWORD len;
	int i;

	loadshit("ntdll.dll", ".text", &addr, &len);
	find_rets(addr, len);

	print_gadgets();
	return 0;
}
*/


void print_gadgets(gadgets_t *gadgets){
	gadget_t **gadget_list;
	int i;

	gadget_list = malloc(gadgets->node_count * sizeof(gadget_t *));

	i = 0;
	arrayize_gadgets(gadget_list, &i, gadgets->root);

	//qsort(gadget_list, gadgets->node_count, sizeof(gadget_t *), count_cmp);
	qsort(gadget_list, gadgets->node_count, sizeof(gadget_t *), inst_cmp);

	for(i = 0; i < gadgets->node_count; i++){
		print_decode_gadget(gadget_list[i]);
	}
}

void loadshit(char *lib, char *seg, char **addr, DWORD *len){
	int i;
	char *lib_base = LoadLibrary(lib);
	DWORD sig_offset = *(DWORD*)&lib_base[0x3c];
	DWORD sig = *(DWORD*)&lib_base[sig_offset];
	WORD num_sect = *(WORD*)&lib_base[sig_offset+4+2];
	WORD opthdr_size = *(WORD*)&lib_base[sig_offset+4+16];

	printf("%08x\n", sig_offset);
	printf("%08x\n", sig);
	printf("%04x\n", num_sect);
	printf("%04x\n", opthdr_size);

	for(i = sig_offset+4+20+opthdr_size; i < sig_offset+4+20+opthdr_size+num_sect*40; i+=40){
		if(strncmp(&lib_base[i], seg, 8) == 0){
			printf("seg: %s\n", &lib_base[i]);
			printf("Size: %08x\n", *(DWORD*)&lib_base[i+8]);
			printf("Addr: %08x\n", *(DWORD*)&lib_base[i+12]);
			printf("VAdr: %08x\n", *(DWORD*)&lib_base[i+12] + (DWORD)lib_base);
			*addr = (char *)(*(DWORD*)&lib_base[i+12] + lib_base);
			*len = *(DWORD*)&lib_base[i+8];
			return;
		}
	}
}





void print_all_gadgets_btnode(bt_node_t *n){
	if(n == NULL){
		return;
	}
	print_all_gadgets_btnode(n->left);
	print_decode_gadget(n->data);
	print_all_gadgets_btnode(n->right);
}

int print_decode_gadget(gadget_t *g){
	int i;
	instruction_t *inst;
	char *next_p = g->code;
	char *p = NULL;

	while(next_p != p){
		p = next_p;
		for(i = 0; i < ret_count; i++){
			inst = &instruction_list_x86_ret[i];
			if(memcmp(p, inst->code, inst->length) == 0){
				if(p == g->code){
					printf("%08x: %5d %s\n", g->offset, g->count, inst->str);
				} else {
					printf(" | %s\n", inst->str);
				}
				return 0;
			}
		}
		if(p == next_p){
			for(i = 0; i < inst_count; i++){
				inst = &instruction_list_x86[i];
				if(memcmp(p, inst->code, inst->length) == 0){
					next_p = p + inst->length;
					if(p == g->code){
						printf("%08x: %5d %s", g->offset, g->count, inst->str);
					} else {
						printf(" | %s", inst->str);
					}
				}
			}
		}
	}
	printf("error?\n");
	return 0;
}

void arrayize_gadgets(gadget_t **gadget_list, int *c, bt_node_t *n){
	if(n == NULL){
		return;
	}

	arrayize_gadgets(gadget_list, c, n->left);
	gadget_list[(*c)++] = n->data;

	arrayize_gadgets(gadget_list, c, n->right);
}

int count_cmp(void *a, void *b){
	return (*(gadget_t **)a)->count - (*(gadget_t **)b)->count;
}

int inst_cmp(void *a, void *b){
	return (*(gadget_t **)a)->instructions - (*(gadget_t **)b)->instructions;
}
