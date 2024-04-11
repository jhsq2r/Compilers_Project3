#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"

typedef struct {
    char* str;
    int num;
} Element;

typedef struct {
    Element items[100];
    int top;
} Stack;

void initializeStack(Stack* stack) {
    stack->top = -1;
}

void push(Stack* stack, char* str, int num) {
    
    Element newItem;
    newItem.str = strdup(str);
    newItem.num = num;

    stack->items[++stack->top] = newItem;
}


int find(Stack* stack, char* c) {
    for (int i = stack->top; i >= 0; i--) {
        if (strcmp(stack->items[i].str, c) == 0) {
            //printf("Element containing '%s' found: %s, %d\n", c, stack->items[i].str, stack->items[i].num);
            return 0; // Found
        }
    }

    //printf("Element containing '%s' not found in the stack.\n", c);
    return 1; // Not found
}

void popN(Stack* stack, int n) {
    if (n <= 0) {
        //printf("Invalid value of N\n");
        return;
    }

    while (n > 0 && stack->top >= 0) {
        free(stack->items[stack->top].str);
        stack->top--;
        n--;
    }
}

void popAllAndFree(Stack* stack) {
    while (stack->top >= 0) {
        
        free(stack->items[stack->top].str);
        stack->top--; 
    }
}
int varcounter = 0;
void semantics(struct Node* root, Stack* stack){
        if(root != NULL){
                if(strcmp(root->nodeName,"programExtended") == 0){
                    varcounter = 0;
                    //printf("In programExtended\n");
                }else if(strcmp(root->nodeName, "vars") == 0 && strcmp(root->tk1->tokenInstance, "create") == 0){
                    //printf("In vars\n");
                    //check if already declared
                    //if not add it to stack
                    if(find(stack, root->tk2->tokenInstance) == 1){
                        push(stack,root->tk2->tokenInstance,0);//mark with no initial value
                        //printf("adding %s to stack\n", root->tk2->tokenInstance);
                        varcounter = varcounter + 1;
                        //printf("varcounter: %d\n", varcounter);
                    }else{//if yes call error
                        printf("Error: %s already declared in scope\n", root->tk2->tokenInstance);
                        return;
                    }
                    //increment block var counter
                    
                }else if(strcmp(root->nodeName, "varsExtended") == 0 && strcmp(root->tk1->tokenInstance, ":=")){
                    //printf("In varsExtended\n");
                    //edit top of stack to show that the variable was initialized with a value
                    stack->items[stack->top].num = 1;
                }else if(strcmp(root->nodeName, "block") == 0){
                    //printf("In block\n");
                    //pop stack varcounter times to clean scope
                    //printf("Popping %d times\n", varcounter);
                    popN(stack, varcounter);
                    //reset varcounter
                    varcounter = 0;
                }else if(strcmp(root->nodeName, "stat--assignSet") == 0 || strcmp(root->nodeName, "stat--assign") == 0){
                    //printf("In assign\n");
                    //check if variables are in scope
                    if(strcmp(root->nodeName, "stat--assignSet") == 0){
                        if(find(stack, root->tk2->tokenInstance) == 1){
                            printf("Error: %s Not in scope\n", root->tk2->tokenInstance);
                            return;
                        }else{
                            //add something to see if it has been initialized with a value
                        }
                    }else{
                        if(find(stack, root->tk1->tokenInstance) == 1){
                            printf("Error: %s Not in scope\n", root->tk2->tokenInstance);
                            return;
                        }else{
                            //add something to see if it has been initialized with a value
                        }
                    }
                    
                }else if(strcmp(root->nodeName, "r") == 0 && strcmp(root->tk1->idTk,"IDENT_TK") == 0){
                    //printf("In r\n");
                    //check if variable is in scope
                    if(find(stack, root->tk1->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk1->tokenInstance);
                        return;
                    }else{
                        //add something to see if it has been initialized with a value
                    }
                    //error if not
                }else if(strcmp(root->nodeName, "stat--in") == 0){
                    //printf("In in\n");
                    //check if tk2 variable is in scope
                    if(find(stack, root->tk2->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk2->tokenInstance);
                        return;
                    }else{
                        //add something to see if it has been initialized with a value
                    }
                }
                
                semantics(root->left, stack);
                semantics(root->leftmiddle, stack);
                semantics(root->rightmiddle, stack);
                semantics(root->right, stack);
        }
}


void staticSemantics(struct Node* root){
    Stack stack;
    initializeStack(&stack);

    printf("STATIC SEMANTICS ERRORS:\n");
    semantics(root, &stack);

    popAllAndFree(&stack);
}
