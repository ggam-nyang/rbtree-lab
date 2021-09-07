#include "rbtree.h"
#include <malloc.h>
#include <unistd.h>


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
  }

  else
  {
    node_t *new = (node_t*)calloc(sizeof(node_t), 1);
    node_insert(t -> root, new, key);
    insert_color(t, new);
  }

  return t -> root;
}

// insert 함수 (루트, 새 노드, 새 키값 ) 재귀를 하기 위함, 먼저 노드를 배정함
node_t *node_insert(node_t *root, node_t *new, const key_t key) {
  if (root == NULL) {
    new -> parent = root;
    new -> key = key;
    new -> left = NULL;
    new -> right = NULL;
    new -> color = RBTREE_RED;
    return new;
  }

  if (root -> key > key) {
    root -> left = node_insert(root -> left, new, key);
    root -> left -> parent = root;
  }
  else {
    root -> right = node_insert(root -> right, new, key);
    root -> right -> parent = root;
  }
  return root;
}

// insert 후 color를 조정하는 함수 (루트, 새 노드)
void insert_color(rbtree *t, node_t *new) {
  node_t *new_parent = NULL;
  node_t *new_grandpa = NULL;

  // 부모가 빨강이고, 새로운 녀석이 아직 빨강이고!! 루트가 아닐 때
  while ((new != t -> root) && (new -> color != RBTREE_BLACK) && (new -> parent -> color == RBTREE_RED))
  {
    new_parent = new -> parent;
    new_grandpa = new -> parent -> parent;

    // 케이스1 : 부모가 왼쪽 자식일 때
    if (new_parent == new_grandpa -> left) 
    {
      node_t *new_uncle = new_grandpa -> right;
      // 케이스 1-1 : 삼촌이 빨강! (부모도 빨강인 상황)
      if (new_uncle != NULL && new_uncle -> color == RBTREE_RED)
      {
        new_grandpa -> color = RBTREE_RED;
        new_parent -> color = RBTREE_BLACK;
        new_uncle -> color = RBTREE_BLACK;
        new = new_grandpa;
      }
      // 케이스 1-2 : 삼촌이 빨강이 아니고 내가 오른쪽 자식일 때 (왼쪽 로테이션이 필요할 때)
      else 
      {
        if (new == new_parent -> right)
        {
          left_rotate(t ,new_parent);
          new = new_parent;
          new_parent = new -> parent;
        }
        // 이제 다시 오른쪽으로 돌려줘야 함. linear가 만들어진 상태 (나는 왼쪽 자식이고)
        right_rotate(t ,new_grandpa);
        int temp_color = new_parent -> color;
        new_parent -> color = new_grandpa -> color;
        new_grandpa -> color = temp_color;
        new = new_parent;
      }
    }
    // 케이스2 : 부모가 오른쪽 자식일 때
    else
    {
      node_t *new_uncle = new_grandpa -> left;
      // 케이스 2-1 : 삼촌이 빨강
      if ((new_uncle != NULL) && (new_uncle -> color == RBTREE_RED))
      {
        new_grandpa -> color = RBTREE_RED;
        new_parent -> color = RBTREE_BLACK;
        new_uncle -> color = RBTREE_BLACK;
        new = new_grandpa;
      }
      else
      {
        // 케이스 2-2 : 삼촌이 까망, 그리고 내가 왼쪽 자식 -> 우회전 필요
        if (new == new_parent -> left)
        {
          right_rotate(t, new_parent);
          new = new_parent;
          new_parent = new -> parent;
        }
        // 이제 다시 왼쪽으로 돌려야 함

        left_rotate(t, new_grandpa);
        int temp_color = new_parent -> color;
        new_parent -> color = new_grandpa -> color;
        new_grandpa -> color = temp_color;
        new = new_parent;
      }
      
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

void left_rotate(rbtree *t, node_t *temp)
{
  node_t *right_c = temp -> right;
  temp -> right = right_c -> left;
  if (temp -> right)
    temp -> right -> parent = temp;
  right_c -> parent = temp -> parent;
  // 이 부분에서 에러가 날 수 도!!!! EEEERRRRRRRRRRRRRRRROOOOOOOOOOORRRRRRRRRRRRRRR  (왜냐하면 rbtree *이 아닌 *root를 인자로 줬으므로! 괜찮을 것 같긴 함)
  if (!temp -> parent)
    t -> root = right_c;
  else if (temp == temp -> parent -> left)
    temp -> parent -> left = right_c;
  else
    temp -> parent -> right = right_c;
  
  right_c -> left = temp;
  temp -> parent = right_c;
}

void right_rotate(rbtree *t ,node_t *temp)
{
  node_t *left_c = temp -> left;
  temp -> left = left_c -> right;
  if (temp -> left)
    temp -> left -> parent = temp;
  left_c -> parent = temp -> parent;
  // 이 부분에서 에러가 날 수 도!!!! EEEERRRRRRRRRRRRRRRROOOOOOOOOOORRRRRRRRRRRRRRR  (왜냐하면 rbtree *이 아닌 *root를 인자로 줬으므로! 괜찮을 것 같긴 함)
  if (!temp -> parent) 
    t -> root = left_c;
  else if (temp == temp -> parent -> left)
    temp -> parent -> left = left_c;
  else
    temp -> parent -> right = left_c;
  
  left_c -> right = temp;
  temp -> parent = left_c;

}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  if (t -> root == NULL)
    return NULL;
  
  node_t *curr = t -> root;
  while (curr != NULL)
  {
    if (curr -> key > key)
      curr = curr -> left;
    else if (curr -> key < key)
      curr = curr -> right;
    else
      return curr;
  }


  return NULL;
}

void print_inorder(node_t *root) {
  
  if (root == NULL)
    return ;
  print_inorder(root -> left);
  printf("%d   %d\n", root -> key, root -> color);
  print_inorder(root -> right);
}


node_t *rbtree_min(const rbtree *t) {
  node_t *temp = t -> root;
  if (temp == NULL)
    return NULL;
  while (temp -> left != NULL)
  {
    temp = temp -> left;
  }
  
  return temp;
}



node_t *rbtree_max(const rbtree *t) {
  node_t *temp = t -> root;

  if (temp == NULL)
    return NULL;
  
  while (temp -> right != NULL)
  {
    temp = temp -> right;
  }

  return temp;
}

node_t *find_min(node_t *temp) {
  if (temp -> left)
    return find_min(temp -> left);
  else
    return temp;
}

node_t *find_child(node_t *p) {
  // 자식이 둘일 때
  if (p -> left != NULL && p -> right != NULL)
    return find_min(p -> right);
  // 자식이 없을 때
  if (p -> left == NULL && p -> right == NULL)
    return NULL;
  // 자식이 하나
  if (p -> left != NULL)
    return p -> left;
  else
    return p -> right;
}

node_t *find_sibling(node_t *new) {
  if (new -> parent == NULL)
    return NULL;
  if (new -> parent -> left == new)
    return new -> parent -> right;
  return new -> parent -> left;
}

void *fix_double_black(rbtree *t, node_t *new) {
  if (new == t -> root)
    return 0;
  node_t *new_sibl = find_sibling(new);
  node_t *new_parent = new -> parent;

  if (new_sibl == NULL)
  // 다시 생각해보기
    fix_double_black(t, new_parent);
  else
  {
    // 형제가 빨강!!
    if (new_sibl -> color == RBTREE_RED) {
      new_parent -> color = RBTREE_RED;
      new_sibl -> color = RBTREE_BLACK;
      if (new_sibl -> parent -> left == new_sibl)
        right_rotate(t, new_parent);
      else
        left_rotate(t, new_parent);
    fix_double_black(t, new);
    }
    // 형제가 까망!! 첫번째 : 빨간 자식이 하나라도 있따.
    else
    {
      if ((new_sibl -> left != NULL && new_sibl -> left -> color == RBTREE_RED) || (new_sibl -> right != NULL && new_sibl -> right -> color == RBTREE_RED))
      {
        if (new_sibl -> left != NULL && new_sibl -> left -> color == RBTREE_RED)
        {
          // left left (형제는 왼쪽 자식이고, 형제의 빨간 자식도 왼쪽 자식)
          if (new_sibl -> parent -> left == new_sibl)
          {
            new_sibl -> left -> color = new_sibl -> color;
            new_sibl -> color = new_parent -> color;
            right_rotate(t, new_parent);
          }
          // right left (형제는 오른쪽 자식이고, 형제의 빨간 자식은 오른쪽)
          else
          {
            new_sibl -> left -> color = new_parent -> color;
            right_rotate(t, new_sibl);
            left_rotate(t, new_parent);
          }
        }
        // 형제의 오른쪽 자식이 빨강!! (new_sibl -> right != NULL && new_sibl -> right -> color == RBTREE_RED)
        else
        {
          // left right (형제는 왼쪽 자식이고, 형제의 빨간 자식은 오른쪽)
          if (new_sibl -> parent -> left == new_sibl)
          {
            new_sibl -> right -> color = new_parent -> color;
            left_rotate(t, new_sibl);
            right_rotate(t, new_parent);
          }
          // right right (형제는 오른쪽 자식, 형제의 빨간 자식은 오른쪽)
          else
          {
            new_sibl -> right -> color = new_sibl -> color;
            new_sibl -> color = new_parent -> color;
            left_rotate(t, new_sibl);
          }
        }
        new_parent -> color = RBTREE_BLACK;   /// 읭??
      }
      // 형제 자식이 빨강이다의 예외!!! 즉, 형제의 자식은 모두 블랙!!!
      else
      {
        new_sibl -> color = RBTREE_RED;
        if (new_parent -> color == RBTREE_BLACK)
          fix_double_black(t, new_parent);
        else
          new_parent -> color = RBTREE_BLACK;
      }
    }
  }
  return 0;
}

int rbtree_erase(rbtree *t, node_t *new) {
  node_t *new_child = find_child(new);
  node_t *new_parent = new -> parent;
  // 자식이 없다면
  if (new_child == NULL) {
    if (new == t -> root) {
      t -> root = NULL;
      return 0;
    }
    // 자식은 없고, 지울 대상이 root는 아닐 때!
    else {
      // 그런데 부모 자식이 둘 다 까망!
      if ((new -> color == RBTREE_BLACK) && (new_child -> color == RBTREE_BLACK || new_child == NULL)) {
        fix_double_black(t, new);
      }
      // 색이 달라!! 부모, 자식 중 하나는 빨강, 근데 자식이 없네? 그럼 삼촌만 빨갛게!!
      else {
        node_t *new_sibl = find_sibling(new);
        if (new_sibl != NULL)
          new_sibl -> color = RBTREE_RED;
      }
    // v를 삭제해줘야 함
    }
    if (new -> parent -> left == new)
      new_parent -> left = NULL;
    else
      new_parent -> right = NULL;
    free(new);
    return 0;
  }
  // 자식이 하나 있을 때
  if (new -> left == NULL || new -> right == NULL) {
    // new가 root 라면!
    if (new == t -> root) {
      new -> key = new_child -> key;
      new -> left = NULL;
      new -> right = NULL;
      free(new_child);
    }
    // new를 삭제하고, child를 위로 올려준다!`
    else {
      if (new -> parent -> left == new) {
        new_parent -> left = new_child;
      }
      else {
        new_parent -> right = new_child;
      }
      free(new);
      new_child -> parent = new_parent;
      if ((new -> color == RBTREE_BLACK) && (new_child -> color == RBTREE_BLACK || new_child == NULL))
        fix_double_black(t, new_child);
      else
        new_child -> color = RBTREE_BLACK;
    }
    return 0;
  }

  // 자식이 두개일 경우!
  int temp;
  temp = new -> key;
  new -> key = new_child -> key;
  new_child -> key = temp;
  rbtree_erase(t, new_child);



  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
