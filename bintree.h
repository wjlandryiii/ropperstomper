#ifndef BINTREE_H
#define BINTREE_H

typedef struct bt_node bt_node_t;
typedef struct bintree bintree_t;

struct bintree {
	bt_node_t *root;
	int node_count;
};


struct bt_node {
	void *data;
	bt_node_t *left;
	bt_node_t *right;
};


bintree_t *alloc_bintree(bt_node_t *root);
void bintree_add_left(bintree_t *tree, bt_node_t *parent, bt_node_t *child);
void bintree_add_right(bintree_t *tree, bt_node_t *parent, bt_node_t *child);
void bintree_remove(bintree_t *tree, bt_node_t *victim);

bt_node_t *alloc_bt_node(void *data, bt_node_t *left, bt_node_t *right);

#endif
