#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include"stack.h"


int
main(int argc, char *argv[])
{
    int i, N;
    char *a;
    stack_t stack;

    a   = argv[1];
    N   = strlen(a);

    stack = stack_new();

    
    for(i = 0; i < N; i++){
        if(')' == a[i]){
            char op = (char)stack_pop(stack);
            printf("%c ", op);
        }

        if('+' == a[i] ||
           '-' == a[i] ||
           '*' == a[i] ||
           '/' == a[i]){
            stack_push(stack, (void *)a[i]);
        }

        if(a[i] >= '0' &&
           a[i] <= '9'){
            printf("%c ", a[i]);
        }
    }

    printf("\n");
    return 0;
}
