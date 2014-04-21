#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gadgets.h"
#include "bintree.h"

//bintree_t gadgets = { .root = NULL };

gadget_t *alloc_gadget(int offset, char *code, int length, int instructions){
	gadget_t *g = malloc(sizeof(gadget_t));
	assert(g != NULL);
	g->code = malloc(length);
	assert(g->code != NULL);
	memcpy(g->code, code, length);
	g->length = length;
	g->offset = offset;
	g->count = 1;
	g->instructions = instructions;
	return g;
}

void free_gadget(gadget_t *victim){
	free(victim->code);
	free(victim);
}


gadgets_t *alloc_gadgets(){
	gadgets_t *gadgets;

   	gadgets = malloc(sizeof(gadgets_t));
	assert(gadgets != NULL);

	gadgets->root = NULL;
	gadgets->node_count = 0;
	return gadgets;
}


void insert_gadget(gadgets_t *gadgets, int offset, char *code, int length, int instructions){
	bt_node_t *n = gadgets->root;
	gadget_t *g = NULL;
	int diff;

	while(n != NULL){
		g = (gadget_t *)n->data;

		if(g->length == length){
			diff = memcmp(g->code, code, length);
			if(diff == 0){
				//direct hit
				g->count++;
				return;
			} else if (diff < 0){
				if(n->left == NULL){
					bintree_add_left(gadgets, n, alloc_bt_node(alloc_gadget(offset, code, length, instructions),NULL,NULL));
					return;
				} else {
					n = n->left;
				}
			} else {
				if(n->right == NULL){
					bintree_add_right(gadgets, n, alloc_bt_node(alloc_gadget(offset, code, length, instructions),NULL,NULL));
					return;
				} else {
					n = n->right;
				}
			}
		} else if(g->length < length){
			if(n->left == NULL){
				bintree_add_left(gadgets, n, alloc_bt_node(alloc_gadget(offset, code, length, instructions),NULL,NULL));
				return;
			} else {
				n = n->left;
			}
		} else {
			if(n->right == NULL){
				bintree_add_right(gadgets, n, alloc_bt_node(alloc_gadget(offset, code, length, instructions),NULL,NULL));
				return;
			} else {
				n = n->right;
			}
		}
	}
	// root MUST be NULL
	assert(gadgets->root == NULL);
	gadgets->root = alloc_bt_node(alloc_gadget(offset, code, length, instructions),NULL,NULL);
	gadgets->node_count++;
}

