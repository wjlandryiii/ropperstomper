#include <stdlib.h>
#include <assert.h>
#include "bintree.h"

bintree_t *alloc_bintree(bt_node_t *root){
	bintree_t *bt = malloc(sizeof(bintree_t));
	assert(bt != NULL);
	bt->root = root;
	bt->node_count = 0;
	return bt;
}

void bintree_add_left(bintree_t *tree, bt_node_t *parent, bt_node_t *child){
	assert(tree != NULL);
	assert(parent != NULL);
	parent->left = child;
	tree->node_count++;
}

void bintree_add_right(bintree_t *tree, bt_node_t *parent, bt_node_t *child){
	assert(tree != NULL);
	assert(parent != NULL);
	parent->right = child;
	tree->node_count++;
}

void bintree_remove(bintree_t *tree, bt_node_t *victim){
	assert(tree != NULL);
	assert(victim != NULL);
	tree->node_count--;
}

bt_node_t *alloc_bt_node(void *data, bt_node_t *left, bt_node_t *right){
	bt_node_t *n = malloc(sizeof(bt_node_t));
	assert(n != NULL);
	n->data = data;
	n->left = left;
	n->right = right;
	return n;
}

