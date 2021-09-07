#include "rbtree.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    rbtree *t = new_rbtree();
    for (i = 0; i < 100000000000000; i++)
    {
        rbtree_insert(t, i);
    }
        pause();
        delete_rbtree(t);
    return 0;
}