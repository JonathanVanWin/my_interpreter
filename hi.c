#include <stdio.h>

typedef struct{
    int x;
    int *y;
} Point;

typedef struct{
    int value;
    char name[];
} Var;

Var var[50];
int c=0;

void r(char*);

void f(){
    char name[100];
    r(name);
    var[c].value=22;
    var[c].name=name;
}
void r(char *name){
    name[0]='H';
    name[1]='i';
    name[2]=' ';
    name[3]='J';
    name[4]='o';
    name[5]='h';
    name[6]='n';
    name[7]='\0';
}

Point ps[30];

void func(){
    ps[0].x=1;
    int *ab =malloc(sizeof(int)*3);
    ps[0].y = ab;
    printf("Point 0: x: %d, y: %s", ps[0].x, "Hi");
}

int main(){
    func();
    f();
    printf("\n\nval: %d, name: %s", var[0].value, var[0].name);
}
