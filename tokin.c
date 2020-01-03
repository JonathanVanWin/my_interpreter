#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "syntax_tree.h"

const char keywords[KEYWORD_COUNT][6] = {"var", "print"};

void token_init(Token *token){
    token->value.c='\0';
    token->type=NONE;
}
void printk(Token token, char *end){
    char *format = malloc(2+strlen(end)+1);
    if(!format)
    {
        printf("token.c::printToken: Malloc error.\n");
        free(format);
        return;
    }
    switch(token.type){
        case VAR:
        case PRINT:
        case EQUALS:
        case STRING_LITERAL:
        case IDENTIFIER:
            strncpy(format, "%s", 3);
            strcat(format, end);
            printf(format, token.value.str);
            break;
        case ASSIGNMENT:
        case ADDITION:
        case SUBTRACTION:
        case ASTERISK:
        case SEMI_COLON:
        case LEFT_PARENTHESIS: case  RIGHT_PARENTHESIS:
        case LEFT_BRACKET: case RIGHT_BRACKET:
        case LEFT_BRACE: case RIGHT_BRACE:
            strncpy(format, "%c", 3);
            strcat(format, end);
            printf(format, token.value.c);
            break;
        case NUMBER:
            strncpy(format, "%d", 3);
            strcat(format, end);
            printf(format, token.value.i);
            break;
    }
    free(format);
}

void parseWord(char *str){
    Token **tokens = malloc(500 * sizeof(*tokens));
    int tokenCount = 0;
    int isNum=0, isStr=0, isWord=0, equalsFlag=0, num=0, strInd=0;
    char *s = malloc(100*sizeof(char));
    for(int i=0; ;i++){
        //Finished parsing number
        if(isNum && (*str<48 || *str>57)){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.i=num;
            tokens[tokenCount++]->type=NUMBER;
            isNum=!isNum;
            num=0;
        }
        //Finished parsing word
        if(isWord && (*str<48 || (*str>57 && *str<65) || (*str>90 && *str<97 && *str!=95) || *str>122)){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            s[strInd]='\0';
            strncpy(tokens[tokenCount]->value.str, s, strInd+1);
            for(int j=0; j<KEYWORD_COUNT; j++){
                if(!strncmp(s, keywords[j], strInd+1)){
                    //We defined the keywords array so that the index value is the same as enum
                    tokens[tokenCount++]->type=j;
                    //Reset and also to know that word is keyword and not identifier
                    isWord=!isWord;
                    break;
                }
            }
            //Means the word wasn't a keyword
            if(isWord){
                tokens[tokenCount++]->type=IDENTIFIER;
                isWord=!isWord;
            }
            s[0]='\0';
            strInd=0;
        }
        //Reset flag (only one '=' means ASSIGNMENT)
        if(*str!=61 && equalsFlag){
            equalsFlag=0;
        }
        //Semi colon (end of statement)
        if(*str==59 && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=SEMI_COLON;
        }
        else if(*str=='(' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=LEFT_PARENTHESIS;
        }
        else if(*str==')' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=RIGHT_PARENTHESIS;
        }
        else if(*str=='[' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=LEFT_BRACKET;
        }
        else if(*str==']' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=RIGHT_BRACKET;
        }
        else if(*str=='{' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=LEFT_BRACE;
        }
        else if(*str=='}' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=RIGHT_BRACE;
        }
        //Assignment operator (=)
        else if(*str==61 && !isStr){
            //Two consecutive equal signs, converts ASSIGNMENT token to EQUALS token
            if(equalsFlag){
                strncpy(tokens[tokenCount-1]->value.str, "==", 3);
                tokens[tokenCount]->type=EQUALS;                
            }
            //Not consecutive (maybe white space)
            else{
                tokens[tokenCount] = malloc(sizeof(**tokens));
                tokens[tokenCount]->value.c=*str;
                tokens[tokenCount++]->type=ASSIGNMENT;
                //First equals sign
                equalsFlag=1;
            }
        }
        //Addition operator (+)
        else if(*str=='+' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=ADDITION;
        }
        //Subtraction operator (-)
        else if(*str=='-' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=SUBTRACTION;
        }
       //Asterisk operator (* Depends on context)
        else if(*str=='*' && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=ASTERISK;
        }
        //Number
        else if(*str>=48 && *str<=57 && !isStr){
            if(isWord){
                s[strInd++]=(*str);
            }
            else{
                isNum=1;
                num=num*10+(*str-48);
            }
        }
        //White space
        else if(*str==32 && !isStr){
            str++;
            continue; 
        }
        //String started or ended (\")
        else if(*str==34){
            //String ended
            if(isStr){
                tokens[tokenCount] = malloc(sizeof(**tokens));
                s[strInd]='\0';
                strncpy(tokens[tokenCount]->value.str, s, strInd+1);
                tokens[tokenCount++]->type=STRING_LITERAL;
                s[0]='\0';
                strInd=0;
            }
            isStr=!isStr;
        }
        //Is word (Identifier, Keyword)
        else if(!isStr && ((*str>=65 && *str<=90) || (*str>=97 && *str<=122) || *str==95)){
            isWord=1;
            s[strInd++]=(*str);
        }
        else if(isStr){
            s[strInd++]=(*str);
        }

        if(!(*str))
            break;
        str++;
    }
    Tree *root = malloc(sizeof(Tree)); 
    tree_init(root, 1); 
    Tree *current = root;
    Tree **addPtrs = malloc(10*sizeof(Tree*));
    int flagInc=0, addIndex =-1;
    for(int i=0; i<tokenCount; i++){
        switch(tokens[i]->type){
            case NUMBER:
                addChild(current, tokens[i]);
                break;
            case STRING_LITERAL:
                break;
            case ADDITION:
                if(addIndex==-1){
                    addIndex=0;
                    if(current->token.type==NONE){
                        current->token=tokens[i]->type;                       
                        addPtrs[addIndex]=current;
                    }
                    tree_init(addPtrs[addIndex], 0);
                }
                root->token=*tokens[i]; 
            case SEMI_COLON: 
                 break;
        }   
    }
    printree(root);
    printf("Result: %d", execute(root));
    /*
    for(int i=0; i<tokenCount; i++){
        switch(tokens[i]->type){
            case NUMBER:
                printf("%d\n", tokens[i]->value.i);
                // printf("Type: NUMBER, Value: %d\n", tokens[i]->value.i);
                break;
            case STRING_LITERAL:
                printf("%s\n", tokens[i]->value.str);
                //printf("Type: STRING_LITERAL, Value: %s\n", tokens[i]->value.str);
                break;
            case IDENTIFIER:
                printf("Type: IDENTIFIER, Value: %s\n", tokens[i]->value.str);
                break;
            case VAR:
                printf("Type: VAR, Value: %s\n", tokens[i]->value.str);
                break; 
            case PRINT:
                printf("Type: PRINT, Value: %s\n", tokens[i]->value.str);
                break; 
            case SEMI_COLON:
                printf("Type: SEMI_COLON, Value: %c\n", tokens[i]->value.c);
                break; 
            case EQUALS:
                printf("Type: EQUALS, Value: %s\n", tokens[i]->value.str);
                break;
            case ASSIGNMENT:
                printf("Type: ASSIGNMENT, Value: %c\n", tokens[i]->value.c);
                break;  
            case ADDITION:
                printf("Type: ADDITION, Value: %c\n", tokens[i]->value.c);
                break; 
            default:
                printf("Not a valid type\n");
        }
    }  */  
    free(tokens);
}

int main(){ 
    char line[400];
    while(1){
        printf(">>> ");
        gets(line);
        parseWord(line);
    }
}
