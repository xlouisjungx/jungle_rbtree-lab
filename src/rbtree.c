#include "rbtree.h"
#include <stdlib.h>

void Left_RoTate(rbtree *T, node_t *x);
void Right_RoTate(rbtree *T, node_t *x);
void Insert_FixUp(rbtree *T, node_t *z);
void TransPlant(rbtree *T, node_t *u, node_t *v);
void Delete_FixUp(rbtree *T, node_t *x);
node_t *successor(rbtree *t, node_t *s);

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

void delete_node(rbtree *t, node_t *node) {
  // TODO: reclaim the tree nodes's memory
  if(node == t->nil) return;
  delete_node(t, node->left);
  delete_node(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t, t->root);
  free(t->nil);
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

  Insert_FixUp(t, Nnode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur != t->nil) {
    if(key == cur->key) return cur; // 동일한 key 값 찾으면 반환
    else if(key < cur->key) cur = cur->left;
    else cur = cur->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->left != t->nil) cur = cur->left;

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->right != t->nil) cur = cur->right;

  return cur;
}

node_t *successor(rbtree *t, node_t *s){
  node_t *cur = s;

  while (cur->left != t->nil){
    cur = cur->left;
  }
  
  return cur;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t *y, *x;  // y : 삭제될 노드, x : 삭제 후 트리 조정을 해야 할 위치(FixUp 함수의 시작점.)
  y = z;
  color_t y_original_color = y->color; // y_original_color : 삭제 전의 y 의 색을 저장. 검은색 노드를 삭제하면 트리의 규칙을 위반하기 떄문에, 미리 저장!
  
  // 삭제의 3가지 case들

  // Case 1: z의 왼쪽 자식이 NIL일 때
  if(z->left == t->nil) {
    x = z->right; // FixUp 시작 지점 갱신
    TransPlant(t, z, z->right); // 왼쪽 자식이 없으므로, 오른쪽만 연결!
  }

  // Case 2: z의 오른쪽 자식이 NIL일 때
  else if(z->right == t->nil) {
    x = z->left;
    TransPlant(t, z, z->left);
  } // Case 1과 반대로 오른쪽 자식이 없으니 왼쪽 자식을 연결.

  // Case3: z가  양쪽 자식이 모두 있을 떄
  else {
    y = successor(t, z->right); // z의 후계자를 찾아라!
    y_original_color = y->color; // 삭제된 노드의 색이 검정이면, FixUp을 위해 미리 저장.
    x = y->right; // 후계자 y를 삭제할 것이기 떄문에, 그의 오른쪽으로 갱신

    // 후계자가 z의 자식이 아닐떄, 재구성하자!!
    if(y->parent != z) {
      TransPlant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    else x->parent = y;

    // y를 z의 자리로 올려서 교체
    TransPlant(t, z, y);
    
    // z의 왼쪽 자식을 y의 왼쪽 자식으로 연결
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  // RB Tree의 속성을 위반하면 큰일나니까, 확인!
  if(y_original_color == RBTREE_BLACK) 
    Delete_FixUp(t, x); // x는 FixUp의 시작 위치를 넘김

  // 삭제된 원래 노드 z의 메모리 해제!
  free(z);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *stack_arr[n];
  node_t *cur = t->root;
  int i = 0;
  while (cur != t->nil) {
    stack_arr[i] = cur;
    i++;
    cur = cur->left;
  }
  int j = 0;
  while (i > 0 && j < n) {
    i--;
    node_t *temp = stack_arr[i];
    arr[j] = temp->key;
    j++;
    if (temp->right != t->nil) {
      cur = temp->right;
      while (cur != t->nil) {
        stack_arr[i] = cur;
        i++;
        cur = cur->left;
      }
    }
  }
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

void Right_RoTate(rbtree *T, node_t *y) {
  node_t *x;

  x = y->left;
  y->left = x->right;

  if(x->right != T->nil) x->right->parent = y;

  x->parent = y->parent;

  if(y->parent == T->nil) T->root = x;

  else if(y == y->parent->right) y->parent->right = x;

  else y->parent->left = x;

  x->right = y;
  y->parent = x;
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

void TransPlant(rbtree *T, node_t *u, node_t *v) {
  if(u->parent == T->nil) T->root = v;
  else if(u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

void Delete_FixUp(rbtree *T, node_t *x) {
  node_t *w;
  while(x != T->root && x->color == RBTREE_BLACK) {
    if(x == x->parent->left) {
      w = x->parent->right;
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
	Left_RoTate(T, x->parent);
	w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
	w->color = RBTREE_RED;
	x = x->parent;
      }
      else {
        if(w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
	  w->color = RBTREE_RED;
	  Right_RoTate(T, w);
	  w = x->parent->right;
	}
	w->color = x->parent->color;
	x->parent->color = RBTREE_BLACK;
	w->right->color = RBTREE_BLACK;
	Left_RoTate(T, x->parent);
	x = T->root;
      }
    }
    else {
      w = x->parent->left;
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
	x->parent->color = RBTREE_RED;
	Right_RoTate(T, x->parent);
	w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
	w->color = RBTREE_RED;
	x = x->parent;
      }
      else {
        if(w->left->color == RBTREE_BLACK) {
	  w->right->color = RBTREE_BLACK;
	  w->color = RBTREE_RED;
	  Left_RoTate(T, w);
	  w = x->parent->left;
	}
	w->color = x->parent->color;
	x->parent->color = RBTREE_BLACK;
	w->left->color = RBTREE_BLACK;
	Right_RoTate(T, x->parent);
	x = T->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
