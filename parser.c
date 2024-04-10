#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"

struct Node {
        char* nodeName;
        struct Token* tk;
        struct Node *left;
        struct Node *leftmiddle;
        struct Node *rightmiddle;
        struct Node *right;
};

struct Node* getNode(char* name){
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
                if (newNode != NULL){
                        newNode->nodeName = strdup(name);
                        newNode->tk = NULL;
                        newNode->left = NULL;
                        newNode->leftmiddle = NULL;
                        newNode->rightmiddle = NULL;
                        newNode->right = NULL;
                        return newNode;
                }
}

void freeTree(struct Node* root){
        if(root != NULL){
                freeTree(root->left);
                freeTree(root->leftmiddle);
                freeTree(root->rightmiddle);
                freeTree(root->right);
                free(root->nodeName);
                free(root->tk);
                free(root);
        }
}

void printInOrder(struct Node* root, FILE *file, int level){
        if(root != NULL){
                printInOrder(root->left, file, level+1);
                fprintf(file,"Level: %d || %s\n", level, root->nodeName);
                printInOrder(root->leftmiddle, file, level+1);
                fprintf(file,"Level: %d || %s\n", level, root->nodeName);
                printInOrder(root->rightmiddle, file, level+1);
                printInOrder(root->right, file, level+1);
        }
}

int tokenNum = 0;

void vars(struct Token* list);
void varsExtended(struct Token* list);
void block(struct Token* list);
void stats(struct Token* list);
void mStat(struct Token* list);
void stat(struct Token* list);
void program(struct Token* list);
void programExtended(struct Token* list);
void pickbody(struct Token* list);
void ro(struct Token* list);
void roExtended(struct Token* list);
void expr(struct Token* list);
void n(struct Token* list);
void nExtended(struct Token* list);
void a(struct Token* list);
void m(struct Token* list);
void r(struct Token* list);

struct Token* currentToken(int x, struct Token* list){

        if(x == 0){
                return list;
        }else{
                int y = 1;
                for(y; y < 100; y++){
                        list = list->next;
                        if(y == x){
                                return list;
                        }
                }
 
        }
}




void parser(struct Token* list){
        //printf("In parser Function...\n");
        program(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"EOF_TK") == 0){
                printf("Success!!!\n");
        }else{
                printf("Error in parser function\n");
        }
        return;
}

void program(struct Token* list){
        //printf("In program Function...Token: %s\n", list->tokenInstance);
        vars(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"tape") == 0){
                tokenNum++;
                programExtended(list);
                return;
        }else{
                //error expected tape
                printf("Error: Expected 'tape' got %s\n", list->tokenInstance);


        }
}

void vars(struct Token* list){
        //printf("In vars Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"create") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        varsExtended(list);
                }
        }else{
                return;
        }
        return;
}

void varsExtended(struct Token* list){
        //printf("In varsExtended Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                tokenNum++;
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,":=") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"INT_TK") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                vars(list);
                                return;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", list->tokenInstance);


                        }
                }else{
                        //error expected INT
                        printf("Error: Expected 'INT' got %s\n", list->tokenInstance);

                }
        }else{
                //error expected ; or := INT ;
                printf("Error: Expected ';' or ':= INT' got %s\n", list->tokenInstance);


        }
}

void programExtended(struct Token* list){
        //printf("In programExtended Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"func") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        block(list);
                        block(list);
                        return;
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", list->tokenInstance);


                }
        }else{
                block(list);
                return;
        }
}

void block(struct Token* list){
        //printf("In block Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"{") == 0){
                tokenNum++;
                vars(list);
                stats(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"}") == 0){
                        tokenNum++;
                        return;
                }else{
                        //errpr expected }
                        printf("Error: Expected '}' got %s\n", list->tokenInstance);


                }
        }else{
                //error expected {
                printf("Error: Expected '{' got %s\n", list->tokenInstance);


        }
}

void stats(struct Token* list){
        //printf("In stats Function...Token: %s\n", list->tokenInstance);
        stat(list);
        mStat(list);
        return;
}

void mStat(struct Token* list){
        //printf("In mStat Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance, "}") == 0){
                return;
        }else{
                stat(list);
                mStat(list);
                return;
        }
}

void stat(struct Token* list){
        //printf("In stat Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"label") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"cin") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"cout") == 0){
                tokenNum++;
                expr(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                        tokenNum++;
                        return;
                }else{
                        //error expected ;
                        printf("Error: Expected ';' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"if") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                        tokenNum++;
                        expr(list);
                        ro(list);
                        expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                tokenNum++;
                                if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"then") == 0){
                                        tokenNum++;
                                        stat(list);
                                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                                tokenNum++;
                                                return;
                                        }else{
                                                //error expected ;
                                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                                        }
                                }else{
                                        //error expected then
                                        printf("Error: Expected 'then' got %s\n", list->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected ']' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected [
                        printf("Error: Expected '[' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"pick") == 0){
                tokenNum++;
                expr(list);
                pickbody(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                        tokenNum++;
                        return;
                }else{
                        //error expected ;
                        printf("Error: Expected ';' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"while") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                        tokenNum++;
                        expr(list);
                        ro(list);
                        expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                tokenNum++;
                                stat(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                        tokenNum++;
                                        return;
                                }else{
                                        //error expected ;
                                        printf("Error: Expected ';' got %s\n", list->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected ']' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected [
                        printf("Error: Expected '[' got %s\n", list->tokenInstance);
                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"repeat") == 0){
                tokenNum++;
                stat(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"until") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                                tokenNum++;
                                expr(list);
                                ro(list);
                                expr(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                        tokenNum++;
                                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                                tokenNum++;
                                                return;
                                        }else{
                                                //error expected ;
                                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                                        }
                                }else{
                                        //error expected ]
                                        printf("Error: Expected ']' got %s\n", list->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected '[' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected until
                        printf("Error: Expected 'until' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"set") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                                tokenNum++;
                                expr(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                        tokenNum++;
                                        return;
                                }else{
                                        //error expected ;
                                        printf("Error: Expected ';' got %s\n", list->tokenInstance);

                                }
                        }else{
                                //error expected =
                                printf("Error: Expected '=' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                        tokenNum++;
                        expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected =
                        printf("Error: Expected '=' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"jump") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"{") == 0){
                block(list);
                return;
        }else{
                //error in stat
                printf("Error: (Stat)Expected something but got %s\n", list->tokenInstance);

        }


}

void pickbody(struct Token* list){
        stat(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,":") == 0){
                tokenNum++;
                stat(list);
                return;
        }else{
                //error expected :
                printf("Error: Expected ':' got %s\n", list->tokenInstance);

        }
}

void ro(struct Token* list){
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"<") == 0){
                tokenNum++;
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,">") == 0){
                tokenNum++;
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                                tokenNum++;
                                return;
                        }else{
                                //error expected .
                                printf("Error: Expected '.' got %s\n", list->tokenInstance);

                        }
                }else{
                        //error expected .
                        printf("Error: Expected '.' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                tokenNum++;
                roExtended(list);
                return;
        }else{
                //error in ro
                printf("Error: (ro)Expected operator but got %s\n", list->tokenInstance);

        }
}

void roExtended(struct Token* list){
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                tokenNum++;
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"!") == 0){
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                        tokenNum++;
                        return;
                }else{
                        //error expected =
                        printf("Error: Expected '=' got %s\n", list->tokenInstance);

                }
        }else{
                //error expected = or !=
                printf("Error: Expected '=' or '!=' got %s\n", list->tokenInstance);

        }
}

void expr(struct Token* list){
        n(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"-") == 0){
                tokenNum++;
                expr(list);
                return;
        }else{
                return;
        }

}

void n(struct Token* list){
        a(list);
        nExtended(list);
        return;
}

void nExtended(struct Token* list){
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"/") == 0){
                tokenNum++;
                a(list);
                nExtended(list);
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"+") == 0){
                tokenNum++;
                a(list);
                nExtended(list);
                return;
        }else{
                return;
        }
}

void a(struct Token* list){
        m(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"*") == 0){
                tokenNum++;
                a(list);
                return;
        }else{
                return;
        }
}

void m(struct Token* list){
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"^") == 0){
                tokenNum++;
                m(list);
                return;
        }else{
                r(list);
                return;
        }
}

void r(struct Token* list){
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"(") == 0){
                tokenNum++;
                expr(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,")") == 0){
                        tokenNum++;
                        return;
                }else{
                        //error expected )
                        printf("Error: Expected ')' got %s\n", list->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                tokenNum++;
                return;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"INT_TK") == 0){
                tokenNum++;
                return;
        }else{
                //error in r
                printf("Error: Expected 'IDENTIFIER', 'INTEGER', or '(' got %s\n", list->tokenInstance);

        }
}
