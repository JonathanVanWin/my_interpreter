#include <stdio.h>
#include <stdlib.h>

int main(){
    int n = 10;
    int **arr = malloc(n*sizeof(int*));
    for(int i=0; i<n; i++){
        arr[i] = malloc(n*sizeof(int*));
        for(int j=0; j<n; j++){
            arr[i][j] = i*n +j;
        }
    }

    for(int i=0; i<n*n; i++){
        printf("%d, ", **arr);
        (*arr)++;
        if((i+1)%n==0){arr++; printf("\n");}
    }
}
