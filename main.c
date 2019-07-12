#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum{INT, FLOAT, STRING, BOOL, CHAR} DataType;

typedef union{
    int i;
    float f;
    char str[100];
} Data;

typedef struct Var_t{
    Data value;
    DataType type;
    char name[];
} Var;

int readWord(char*, char*, int, int);
int parseNumber(char*);
int parseString(char*, char*);
int getIndexValue(char*);
void parseValue(char*);
Var **vars;
int varCount = 0;
void process(char *str){
    char print[6] = "print";
    char var  [4] = "var";
    //If user entered print command
    if(!strncmp(str, print, 5)){
        str+=6;
        char word[100];
        readWord(str, word, 100, 32);     
        int index = getIndexValue(word);
        if(index!=-1){
            if(vars[index]->type==INT){
                printf("%d\n", vars[index]->value.i);
            }else if(vars[index]->type==STRING){
                printf("%s\n", vars[index]->value.str);
            }
        }else
            printf("There's no var named %s\n", word);
        //printf("VarCount: %d\n", varCount);
    } 
    else if(!strncmp(str, var, 3)){
        str+=4;
        char name[100];
        str+=readWord(str, name, 100, '='); 
        vars[varCount] = malloc(sizeof(**vars)+strlen(name)+1);
        parseValue(str);
        //printf("Length: %d, %s", strlen(name)+1, str);
        memcpy(vars[varCount++]->name, name, strlen(name)+1);
        //printf("%s\n",vars[varCount-1]->name);
    }
    
}

int getIndexValue(char *name){
    for(int i=0; i<varCount; i++){
        //printf("var %d: %s, name: %s\n", i, vars[i]->name, name);
        if(!strcmp(vars[i]->name, name))//This assumes that vars starting the same are the same, strlen(vars[i]->name)))
            return i;
    }
    return -1;
}

void parseValue(char *str){
    while((*str)==32 && (*++str)==32);
    if(*str>=48 && *str<=57){
        vars[varCount]->value.i = parseNumber(str);
        vars[varCount]->type = INT;
    }else if(*str==34){
        parseString(str+1, vars[varCount]->value.str);
        vars[varCount]->type = STRING;
    }
}

int parseNumber(char *str){
    int num=0, startedNum = 0, count=0;
    while((*str)){
        if((*str)>57 || (*str)<48){
           //printf("normal text, curr code: %d", (*str));
            if(startedNum)
                break;
            else{
               //puts(", not started num");
                str++;       
            }
        }else{
            startedNum=1;
            //printf("started, digit: %d", (*str));
            num=num*10+((*str)-48);          
            count++;     
            str++; 
        }
    }
    return num;
}

int parseString(char *str, char *dest){
    int i=0, started = 0;
    for(;!(*str==34 && started || !(*str)); i++){
        //printf("%c", *str);
        dest[started++] = (*str++);
    }
    dest[started]='\0';
    //We return the index of the string after the endChar
    return i+1;
}

int readWord(char *str, char *word, int size, int endChar){
    int i=0, stringStart=0, stringEnd=0, endedWord = 0, started = 0;
    for(;i<size; i++){     
        if((*str)==endChar && (!stringStart|| stringStart && stringEnd)|| !(*str))
            break;
        if((*str)==32){
            if(started)
                endedWord = 1;               
        }
        else if(!endedWord){
            word[started++] = (*str);
        }
        str++;
    }
    word[started]='\0';
    //We return the index of the string after the endChar
    return i+1;
}

int main(){
    vars = malloc(50 * sizeof(*vars));
    char str[400];
    
    while(1){
        printf(">> ");
        gets(str);
        process(str);
    }
}
