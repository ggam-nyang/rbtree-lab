#include "rbtree.h"

#include <malloc.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}




node_t *rbtree_insert(rbtree *t, const key_t key) {
  if (t -> root == NULL)
  {
    t -> root = (node_t*)calloc(sizeof(node_t), 1);
    t -> root -> key = key;
    t -> root -> color = RBTREE_BLACK;
    t -> root -> parent = NULL;
    t -> root -> left = NULL;
    t -> root -> right = NULL;
    return t -> root;    
  }

  else
  {
    if (t -> root -> key > key)
      t -> root -> left = rbtree_insert(t -> root -> left, key);
    else if (t -> root -> key < key)
      t -> root -> right = rbtree_insert(t -> root -> right, key);
  }

  return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(const rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
