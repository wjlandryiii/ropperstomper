#ifndef LLIST_H
#define LLIST_H

typedef struct llist llist_t;
typedef struct ll_node ll_node_t;

struct llist {
	ll_node_t *head;
};

struct ll_node {
	void *data;
	ll_node_t *next;
};

void llist_remove_node(llist_t *list, ll_node_t *victim);

#endif
