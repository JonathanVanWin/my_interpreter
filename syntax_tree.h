#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include "token.h"

typedef struct Tree_t{
    Token token;
    struct Tree_t *child;
    struct Tree_t *next;
} Tree;

void addChildren(Tree *root, Token *children, int length);
Tree *addChild(Tree *root, Token *child);
void printree(Tree *root);
void freeMem(Tree *root);
int execute(Tree *root);
//initToken: 1-Default init, 0-No need to init (already done)
void tree_init(Tree *root, int initToken);
#endif
