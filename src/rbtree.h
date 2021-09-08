#ifndef RBTREE_H_
#define RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

// node_t NIL = {RBTREE_BLACK, NULL, NULL, NULL, NULL};


node_t *rbtree_insert(rbtree *, const key_t);
node_t *node_insert(node_t *, node_t *,const key_t);
void insert_color(rbtree *, node_t *);
void left_rotate(rbtree *, node_t *);
void right_rotate(rbtree *, node_t *);


node_t *rbtree_find(const rbtree *, const key_t);


node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);
node_t *find_min(node_t *);
node_t *find_child(node_t *);
node_t *find_sibling(node_t *);
void *fix_double_black(rbtree *, node_t *);


int rbtree_to_array(const rbtree *, key_t *, const size_t);

int inorder_array(node_t *, key_t *, size_t *);

void delete_node(node_t *);


void print_inorder(node_t *);

#endif  // RBTREE_H_

