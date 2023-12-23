#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/contracts.h"
#include "lib/stacks.h"

void print_elem(genstack_elem e)
{
    printf("%d\n", *((int*) e));
}

int main()
{
    /*Stack Tests*/
    genstack *S = stack_new();
    ASSERT(stack_empty(S));

    int a = 8;
    stack_push(S, (void*) &a);
    ASSERT(stack_size(S) == 1);
    ASSERT(!stack_empty(S));
    stack_print(S, &print_elem);

    int b = *((int*) stack_pop(S));
    ASSERT(stack_size(S) == 0);
    ASSERT(stack_empty(S));
    ASSERT(b == 8);

    int c = 7;
    int d = 6;
    int e = 5;
    stack_push(S, (void*) &c);
    stack_push(S, (void*) &d);
    stack_push(S, (void*) &e);

    stack_print(S, &print_elem);
    stack_free(S, NULL);

    printf("All tests passed!\n");
    return EXIT_SUCCESS;
}
