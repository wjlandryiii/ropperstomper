#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "llist.h"


void llist_remove_node(llist_t *list, ll_node_t *victim){
	ll_node_t **pn;

	pn = &list->head;

	while(*pn != NULL && *pn != victim){
		pn = &((*pn)->next);
	}
	if(*pn == NULL)
		return;
	*pn = (*pn)->next;
}
