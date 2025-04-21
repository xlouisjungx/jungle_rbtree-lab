#include "rbtree.h"
#include <stdlib.h>

void Left_RoTate(rbtree *T, node_t *x);
void Right_RoTate(rbtree *T, node_t *x);
void Insert_FixUp(rbtree *T, node_t *z);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));

  // nill 초기화
  nil->color = RBTREE_BLACK;
  nil->parent = nil->left = nil->right = NULL;

  // root와 nil의 초기값
  p->root = nil;
  p->nil = nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *Nnode = (node_t *)calloc(1, sizeof(node_t));
  Nnode->color = RBTREE_RED;
  Nnode->key = key;
  Nnode->parent = Nnode->left = Nnode->right = t->nil;

  node_t *parent = t->nil;
  node_t *cur = t->root;

  while(cur != t->nil) {
    parent = cur;

    if(key < cur->key) cur = cur->left;
    else cur = cur->right;
  }

  // 부모와 연결
  Nnode->parent = parent;

  if(parent == t->nil) t->root = Nnode;
  else if(key < parent->key) parent->left = Nnode;
  else parent->right = Nnode;

  return t->root;
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

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void Left_RoTate(rbtree *T, node_t *x) {
  node_t *y;

  y = x->right;
  x->right = y->left;

  if(y->left != T->nil) y->left->parent = x;

  y->parent = x->parent;

  if(x->parent == T->nil) T->root = y;

  else if(x == x->parent->left) x->parent->left = y;

  else x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void Right_RoTate(rbtree *T, node_t *x) {
  node_t *y;

  y = x->right;
  x->right = y->left;

  if(y->left != T->nil) y->left->parent = x;

  y->parent = x->parent;

  if(x->parent == T->nil) T->root = y;

  else if(x == x->parent->left) x->parent->left = y;

  else x->parent->right = y;

  Insert_FixUp(T, x);

  y->left = x;
  x->parent = y;
}


void Insert_FixUp(rbtree *T, node_t *z) {
  node_t *y;
  while(z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left) {
      
      y = z->parent->parent->right;
      
      if(y->color == RBTREE_RED) {
	z->parent->color = RBTREE_BLACK;
	y->color = RBTREE_BLACK;
	z->parent->parent->color = RBTREE_RED;
	z = z->parent->parent;
      }
      else {
        if(z == z->parent->right) {
 	    z = z->parent; // Case 2
	    Left_RoTate(T, z);
	}
	z->parent->color = RBTREE_BLACK;
	z->parent->parent->color = RBTREE_RED;
	Right_RoTate(T, z->parent->parent);
      }
    }
    else {
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
	y->color = RBTREE_BLACK;
	z->parent->parent->color = RBTREE_RED;
	z = z->parent->parent;
      }
      else {
        if(z == z->parent->left) {
	    z = z->parent;
	    Right_RoTate(T, z);
	}
    	z->parent->color = RBTREE_BLACK;
	z->parent->parent->color = RBTREE_RED;
	Left_RoTate(T, z->parent->parent);
      }
    }
   }
  T->root->color = RBTREE_BLACK;
}
