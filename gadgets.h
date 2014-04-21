#ifndef GADGETS_H
#define GADGETS_H

#include "bintree.h"

typedef bintree_t gadgets_t;

typedef struct gadget {
	int offset;
	char *code;
	int length;
	int count;
	int instructions;
} gadget_t;

extern bintree_t gadgets;
extern gadget_t *alloc_gadget(int offset, char *code, int length, int instructions);
extern void free_gadget(gadget_t *victim);

extern gadgets_t *alloc_gadgets();
extern void insert_gadget(gadgets_t *gadgets, int offset, char *code, int length, int instructions);


#endif
