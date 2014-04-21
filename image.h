/*
 *  Copyright 2014 Joseph Landry
 *
 *
 */



#ifndef IMAGE_H
#define IMAGE_H

#include "llist.h"

typedef struct section section_t;
typedef struct image image_t;

struct section {
	char *name;
	char *start;
	int size;
	int virtual_offset;
	int virtual_size;
	int executable;
};

typedef enum {
	ARCH_x86
} arch_t;

struct image {
	char *contents;
	int size;
	int type;
	arch_t arch;
	llist_t sections;
};

image_t *load_image(char *filename);

// Win32 PE

typedef struct pe_image_msdos_header pe_image_msdos_header_t;
typedef struct pe_image_file_header pe_image_file_header_t;
typedef struct pe_image_optional_header pe_image_optional_header_t;
typedef struct pe_image_data_directory pe_image_data_directory_t;
typedef struct pe_image_section_header pe_image_section_header_t;

#pragma pack(push, 1)
struct pe_image_msdos_header {  // DOS .EXE header
    unsigned short e_magic;         // Magic number
    unsigned short e_cblp;          // Bytes on last page of file
    unsigned short e_cp;            // Pages in file
    unsigned short e_crlc;          // Relocations
    unsigned short e_cparhdr;       // Size of header in paragraphs
    unsigned short e_minalloc;      // Minimum extra paragraphs needed
    unsigned short e_maxalloc;      // Maximum extra paragraphs needed
    unsigned short e_ss;            // Initial (relative) SS value
    unsigned short e_sp;            // Initial SP value
    unsigned short e_csum;          // Checksum
    unsigned short e_ip;            // Initial IP value
    unsigned short e_cs;            // Initial (relative) CS value
    unsigned short e_lfarlc;        // File address of relocation table
    unsigned short e_ovno;          // Overlay number
    unsigned short e_res[4];        // Reserved words
    unsigned short e_oemid;         // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;       // OEM information; e_oemid specific
    unsigned short e_res2[10];      // Reserved words
    int   e_lfanew;        // File address of new exe header
};

struct pe_image_file_header {
    unsigned short  Machine;
    unsigned short  NumberOfSections;
    unsigned int    TimeDateStamp;
    unsigned int    PointerToSymbolTable;
    unsigned int    NumberOfSymbols;
    unsigned short  SizeOfOptionalHeader;
    unsigned short  Characteristics;
};

struct pe_image_optional_header {
    unsigned short  Magic;
    unsigned char   MajorLinkerVersion;
    unsigned char   MinorLinkerVersion;
    unsigned int    SizeOfCode;
    unsigned int    SizeOfInitializedData;
    unsigned int    SizeOfUninitializedData;
    unsigned int    AddressOfEntryPoint;
	unsigned int    BaseOfCode;
	unsigned int    BaseOfData;
    unsigned int    ImageBase;
    unsigned int    SectionAlignment;
    unsigned int    FileAlignment;
    unsigned short  MajorOperatingSystemVersion;
    unsigned short  MinorOperatingSystemVersion;
    unsigned short  MajorImageVersion;
    unsigned short  MinorImageVersion;
    unsigned short  MajorSubsystemVersion;
    unsigned short  MinorSubsystemVersion;
    unsigned int    Reserved1;
    unsigned int    SizeOfImage;
    unsigned int    SizeOfHeaders;
    unsigned int    CheckSum;
    unsigned short  Subsystem;
    unsigned short  DllCharacteristics;
    unsigned int    SizeOfStackReserve;
    unsigned int    SizeOfStackCommit;
    unsigned int    SizeOfHeapReserve;
    unsigned int    SizeOfHeapCommit;
    unsigned int    LoaderFlags;
    unsigned int    NumberOfRvaAndSizes;
    pe_image_data_directory_t DataDirectory[16];
};

struct pe_image_data_directory {
	unsigned int RVA;
	unsigned int Size;
};

struct pe_image_section_header {
	unsigned char Name[8];
	union {
		unsigned int PhysicalAddress;
		unsigned int VirtualSize;
	} Misc;
	unsigned int VirtualAddress;
	unsigned int SizeOfRawData;
	unsigned int PointerToRawData;
	unsigned int PointerToRelocations;
	unsigned int PointerToLinenumbers;
	unsigned short NumberOfRelocations;
	unsigned short NumberOfLinenumbers;
	unsigned int Characteristics;
};
#pragma pack(pop)

int pe_parse(image_t *image);

#endif
