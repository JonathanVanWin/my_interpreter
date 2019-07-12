#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORD_COUNT 2

typedef enum{
    //Keywords
    VAR, PRINT,

    //Misc
    NUMBER, STRING_LITERAL, IDENTIFIER, SEMI_COLON, ASSIGNMENT, EQUALS, ADDITION
} TokenType;

const char keywords[KEYWORD_COUNT][6] = {"var", "print"};

typedef union{
    int i;
    float f;
    char c;
    char str[100];
} Data;

typedef struct{
    Data value;
    TokenType type;
} Token;

Token **tokens;
int tokenCount = 0;

void parseWord(char *str){
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
        //Assignment operator (=)
        else if(*str==61 && !isStr){
            //Two consecutive equal signs, converts ASSIGNMENT token to EQUALS token
            if(equalsFlag){
                strncpy(tokens[--tokenCount]->value.str, "==", 3);
                tokens[tokenCount++]->type=EQUALS;                
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
        else if(*str==43 && !isStr){
            tokens[tokenCount] = malloc(sizeof(**tokens));
            tokens[tokenCount]->value.c=*str;
            tokens[tokenCount++]->type=ADDITION;
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
    for(int i=0; i<tokenCount; i++){
        switch(tokens[i]->type){
            case NUMBER:
                printf("Type: NUMBER, Value: %d\n", tokens[i]->value.i);
                break;
            case STRING_LITERAL:
                printf("Type: STRING_LITERAL, Value: %s\n", tokens[i]->value.str);
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
    }
}

int main(){
    tokens = malloc(500 * sizeof(*tokens));
    char line[400];
printf("VAR: %d, PRINT: %d", VAR, PRINT);
    while(1){
        printf(">>> ");
        gets(line);
        parseWord(line);
    }
}
