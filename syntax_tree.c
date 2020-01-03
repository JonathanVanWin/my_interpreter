#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "token.h"
#include "syntax_tree.h"

//initToken: 1-Default init, 0-No need to init (already done)
void tree_init(Tree *root, int initToken){
    root->next=NULL;
    root->child=NULL;
    if(initToken){
        token_init(&root->token);
    }
}

void addChildren(Tree *root, Token *children, int length){
    root->child = malloc(sizeof(Tree));
    tree_init(root->child,0);
    //For setting last one to NULL
    Tree **tmp = &root->child;
    for(int i=0; i<length; i++){
        (*tmp)->token = children[i];
        (*tmp)->next  = malloc(sizeof(Tree));
        tree_init((*tmp)->next, 0);
        tmp=&(*tmp)->next;
    }
    *tmp=NULL;
}

Tree *addChild(Tree *root, Token *child){
    if(!root->child){
        root->child = malloc(sizeof(Tree));
        root->child->token = *child;
        tree_init(root->child, 0);
        return root->child;
    } else {
        Tree **tmp = &root->child;
        while(*tmp && (*tmp)->next)
            tmp=&(*tmp)->next;
        (*tmp)->next = malloc(sizeof(Tree));
        (*tmp)->next->token = *child;
        tree_init((*tmp)->next, 0);
        return (*tmp)->next;
    }
}   

void printree(Tree *root){
    if(!root) return;
    if(root->next){
        printk(root->token, ", ");
        printree(root->next);
    } else
        printk(root->token, "\n");
    if(root->child) printree(root->child);    
}

void freeMem(Tree *root){
    Tree *tmp1 = root->child;
    Tree *tmp  = root->child;
    while(tmp){
        tmp1=tmp->next;
        free(tmp);
        tmp=tmp1;
    }
    free(root);
}

int execute(Tree *root){
    int n1, n2;
    Tree *child = root->child;
    switch(root->token.type){
        case ADDITION:
            n1 = child->token.value.i;
            printf("Value: %d\n", n1);//Maybe move next or child, depends
            return n1 + execute(child);
            break;
        case SUBTRACTION:
            n1 = child->token.value.i;
            return n1 - execute(child);
            break;
        default:
            return -1;
    }
    return 2;
} 

/*int main(){
    Tree *root = malloc(sizeof(Tree));
    root->next = NULL;
    root->child= NULL;
    root->token.value.c = '+';
    root->token.type=ADDITION;
    Token *child1 = malloc(sizeof(Token));
    child1->value.i = 3;
    child1->type=NUMBER; 
    Token *child2 = malloc(sizeof(Token));
    child2->value.i = 24;
    child2->type=NUMBER;
    Token *child3= malloc(sizeof(Token));
    child3->type=STRING_LITERAL;
    strncpy(child3->value.str, "Hello World!", 13);
    addChild(root, child1);
    addChild(root, child2);
    addChild(root, child3);
    printree(root);
    //execute(root);
    free(child2); free(child1);// free(child);
    freeMem(root);
}*/
