#include <stdio.h>
();
int main(){
    char *str="Hello";
    printf("%p", str+1);
    printf("657 %s", 657 ? "true" : "false");
    printf("Hello %s", "Hello" ? "true" : "false");
    printf("0 %s", 0 ? "true" : "false");
    printf("\"\" %s", "" ? "true" : "false");
    printf("NULL %s", NULL ? "true" : "false");
}
