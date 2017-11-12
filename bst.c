#include <stdio.h>
#include <stdlib.h>

#define ElemType int
typedef struct BTNode {
    ElemType element;
    struct BTNode *left, *right;
} Node, *Tree;

Tree find(ElemType x, Tree T)
{
    while (T != NULL && x != T->element)
        if (x < T->element)
            T = T->left;
        else
            T = T->right;
    return T;
}

Tree find_min(Tree T)
{
    if (T != NULL)
        while (T->left != NULL)
            T = T->left;
    return T;
}

Tree insert(ElemType x, Tree T)
{
    if (T == NULL) {
        T = (Tree) malloc(sizeof(Node));
        T->element = x;
        T->left = T->right = NULL;
    } else if (x < T->element)
        T->left = insert(x, T->left);
    else if (x > T->element)
        T->right = insert(x, T->right);
    return T;
}

Tree delete(ElemType x, Tree T)
{
    if (T == NULL)
        return NULL;
    if (x < T->element)
        T->left = delete(x, T->left);
    else if (x > T->element)
        T->right = delete(x, T->right);
    else if (T->left && T->right) {
        T->element = find_min(T->right)->element;
        T->right = delete(T->element, T->right);
    } else {
        Tree tmp = T;
        T = (T->left != NULL) ? T->left : T->right;
        free(tmp);
    }
    return T;
}

void in_order(Tree T)
{
    if (T != NULL) {
        in_order(T->left);
        printf("%d\t", T->element);
        in_order(T->right);
    }
}

int main()
{
    Tree T = NULL;
    int n = 16, r;
    unsigned long next = 1;
    while (n--) {
        next = next * 1103515245 + 12345;
        r = (unsigned) (next / 65536) % 32768;
        printf("%d\t", r);
        T = insert(r, T);
    }
    printf("\n");
    in_order(T);
    printf("\n");
    return 0;
}
