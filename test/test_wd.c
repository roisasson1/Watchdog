#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "wd.h"

int FibonacciIterative(int elemnet_index);

int main(int argc, const char** argv)
{
    size_t i = 0;
    WDStart(argc, argv, 1, 5);
    printf("Start Critical Code:\n");
    
    for (i = 0; i < 15; i++)
    {
        sleep(2);
        printf("%d\n", FibonacciIterative(i));
    }

    printf("End Critical Code:\n");
    WDStop();
    
    return 0;
}

int FibonacciIterative(int elemnet_index)
{
    int i = 0;
    int prev = 0;
    int current = 1;
    int next = 0;

    if (elemnet_index <= 1)
    {
        return elemnet_index;
    }

    for ( i = 2; i <= elemnet_index; ++i) 
    {
        next = prev + current;
        prev = current;
        current = next;
    }

    return next;
}
