/*
    На вход программе подается строка, длинной не более 254 символа.
    Символ переноса строки в дерево не заносится.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { N = 256 };

struct Tree {
    char c;
    int n;
    struct Tree *left;
    struct Tree *right;
    struct Tree *parent;
};

struct Tree *Create_tree(char *);
void Add_char(struct Tree **, struct Tree *, char );
void Free_tree(struct Tree **);
void Print_tree(struct Tree *);


int
main(void)
{
    char str[N] = {'\0'};
    fgets(str, N, stdin);
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
    struct Tree *tree_ptr = Create_tree(str);
    Print_tree(tree_ptr);
    Free_tree(&tree_ptr);
    return 0;
}


struct Tree *
Create_tree(char *str)
{
    struct Tree *tree_ptr = NULL;
    for (int i = 0; i < strlen(str); ++i) {
        Add_char(&tree_ptr, NULL, str[i]);
    }
    return tree_ptr;
}


void
Add_char(struct Tree **ptr, struct Tree *parent_ptr, char c)
{
    if (*ptr) {
        if ((*ptr)->c == c) {
            ++((*ptr)->n);
        }
        else if ((unsigned)((*ptr)->c) < (unsigned)c) {
            Add_char(&((*ptr)->right), *ptr, c);
        }
        else {
            Add_char(&((*ptr)->left), *ptr, c);
        }
    }
    else {
        struct Tree *newptr = (struct Tree*)malloc(sizeof(struct Tree));
        newptr->parent = parent_ptr;
        newptr->right = NULL;
        newptr->left = NULL;
        newptr->c = c;
        newptr->n = 1;
        *ptr = newptr;
    }
    return;
}


void
Print_tree(struct Tree *ptr)
{
    if (!ptr) {
        return;
    }
    Print_tree(ptr->left);
    printf("%c %d\n", ptr->c, ptr->n);
    Print_tree(ptr->right);
    return;
}


void
Free_tree(struct Tree **fptr)
{
    struct Tree *ptr = *fptr;
    while (ptr) {
        while (ptr->left || ptr->right) {
            while (ptr->left) {
                ptr = ptr->left;
            }
            while (ptr->right) {
                ptr = ptr->right;
            }
        }
        if (!ptr->parent) {
            free(ptr);
            ptr = NULL;
        }
        else if (ptr->parent->right == ptr) {
            ptr = ptr->parent;
            free(ptr->right);
            ptr->right = NULL;
        }
        else {
            ptr = ptr->parent;
            free(ptr->left);
            ptr->left = NULL;
        }
    }
    *fptr = NULL;
    return;
}

