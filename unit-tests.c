#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/contracts.h"
#include "lib/stacks.h"
#include "lib/squares.h"

void print_elem(genstack_elem e)
{
    printf("%d\n", *((int*) e));
}

void test_stacks()
{
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
}

void test_squares()
{
    square *S = square_new(3);

    square_write(S, 'a', 0, 0);
    char a = square_read(S, 0, 0);
    ASSERT(a == 'a');
    square_print(S);

    square_write(S, 'i', 2, 2);
    char i = square_read(S, 2, 2);
    ASSERT(i == 'i');
    square_print(S);

    square_write(S, 'b', 1, 0);
    square_write(S, 'c', 2, 0);
    square_write(S, 'd', 0, 1);
    square_write(S, 'e', 1, 1);
    square_write(S, 'f', 2, 1);
    square_write(S, 'g', 0, 2);
    square_write(S, 'h', 1, 2);
    square_print(S);

    square_free(S);
}

int main()
{
    test_stacks();
    test_squares();

    printf("All tests passed!\n");
    return EXIT_SUCCESS;
}
