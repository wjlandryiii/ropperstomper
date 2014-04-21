#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"
#include "gadget_search.h"
#include "instructions_x86.h"

void find_gadgets_x86(image_t *image, gadgets_t *gadgets);

gadgets_t *find_gadgets(image_t *image){
	gadgets_t *gadgets;

	gadgets = alloc_gadgets();

	switch(image->arch){
		case ARCH_x86:
			find_gadgets_x86(image, gadgets);
			break;
		default:
			printf("Unknown arch: %d\n", image->arch);
	};

	return gadgets;
}

void find_rets(gadgets_t *gadgets, int section_offset, char *buff, int len);
int inst_search_backwards(gadgets_t *gadgets, int section_offset, char *buff, int len, int pos);
int decode_gadget(gadgets_t *gadgets, int section_offset, char *buff, int len, int pos);


void find_gadgets_x86(image_t *image, gadgets_t *gadgets){
	ll_node_t *p_node;
	section_t *p_section;

	p_node = image->sections.head;

	while(p_node != NULL){
		p_section = p_node->data;
		if(p_section->executable != 0){
			find_rets(gadgets, p_section->virtual_offset, p_section->start, p_section->size);
		}
		p_node = p_node->next;
	}
}


void find_rets(gadgets_t *gadgets, int section_offset, char *buff, int len){
	int i, j;
	char *p;
	char l;
	instruction_t *ret_inst;

	for(i = 0; i < ret_count; i++){
		ret_inst = &instruction_list_x86_ret[i];
		for(j = 0; j < len - ret_inst->length; j++){
			if(memcmp(&buff[j], ret_inst->code, ret_inst->length) == 0){
				if(ret_inst->type == INS_RET){
					inst_search_backwards(gadgets, section_offset, buff, len, j);
				} else {
					decode_gadget(gadgets, section_offset, buff, len, j);
				}
			}
		}
	}
}

int inst_search_backwards(gadgets_t *gadgets, int section_offset, char *buff, int len, int pos){
	int i;
	instruction_t *inst;
	int begin;

	//found_gadget(buff, len, pos);
	decode_gadget(gadgets, section_offset, buff, len, pos);

	for(i = 0; i < inst_count; i++){
		inst = &instruction_list_x86[i];
		begin = pos - inst->length;
		if(begin >= 0){
			if(memcmp(&buff[begin], inst->code, inst->length) == 0){
				inst_search_backwards(gadgets, section_offset, buff, len, begin);
				//return 1;
			}
		}
	}
	return 0;
}


/*

int found_gadget(char *buff, int len, int pos){
	int i,j,p;
	int newp;
	instruction_t *inst;

	printf("%08x: ", (int)&buff[pos]);
	p = -1;
	newp = pos;


	while(newp != p){
		p = newp;
		for(i = 0; i < ret_count; i++){
			inst = &instruction_list_x86_ret[i];
			if(p + inst->length <= len){
				if(memcmp(&buff[p], inst->code, inst->length) == 0){
					printf(" | %s\n", inst->str);
					return 0;
				}
			}
		}
		for(i = 0; i < inst_count; i++){
			inst = &instruction_list_x86[i];
			if(p + inst->length <= len){
				if(memcmp(&buff[p], inst->code, inst->length) == 0){
					printf(" | %s", inst->str);
					newp = p + inst->length;
					break;
				}
			}
		}
	}
	printf(" | ????\n");
	return 0;
}
*/

int decode_gadget(gadgets_t *gadgets, int section_offset, char *buff, int len, int pos){
	int i;
	char *start = &buff[pos];
	int glength;
	char *p;
	char *next_p;
	instruction_t *inst;
	int instructions;

	p = NULL;
	next_p = start;
	instructions = 0;

	while(next_p != p){
		p = next_p;
		for(i = 0; i < ret_count; i++){
			inst = &instruction_list_x86_ret[i];
			if(memcmp(p, inst->code, inst->length) == 0){
				instructions++;
				p += inst->length;
				glength = p - start;
				//add_gadget(buff, len, pos, glength);
				insert_gadget(gadgets, section_offset + (&buff[pos] - buff), &buff[pos], glength, instructions);
				return 0;
			}
		}
		if(p == next_p){
			for(i = 0; i < inst_count; i++){
				inst = &instruction_list_x86[i];
				if(memcmp(p, inst->code, inst->length) == 0){
					instructions++;
					next_p = p + inst->length;
				}
			}
		}
	}
	printf("error?\n");
	return 0;
}

/*
void add_gadget(char *buff, int len, int pos, int glength){
	insert_gadget(pos, &buff[pos], glength);
}
*/

