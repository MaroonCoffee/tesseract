#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/contracts.h"
#include "lib/stacks.h"
#include "lib/squares.h"
#include "lib/cubes.h"

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

void test_cubes()
{
    cube *C = cube_new(2);
    cube_print(C);

    square *a = square_new(2);
    square *b = square_new(2);
    square *c = square_new(2);
    square *d = square_new(2);
    square *e = square_new(2);
    square *f = square_new(2);
    
    cube_free_face(C, 0);
    cube_free_face(C, 1);
    cube_free_face(C, 2);
    cube_free_face(C, 3);
    cube_free_face(C, 4);
    cube_free_face(C, 5);

    square_write(a, 'a', 0, 0);
    square_write(b, 'b', 0, 0);
    square_write(c, 'c', 0, 0);
    square_write(d, 'd', 0, 0);
    square_write(e, 'e', 0, 0);
    square_write(f, 'f', 0, 0);

    cube_write(C, a, 0);
    cube_write(C, b, 1);
    cube_write(C, c, 2);
    cube_write(C, d, 3);
    cube_write(C, e, 4);
    cube_write(C, f, 5);

    square_write(cube_read(C, 0), 'b', 1, 0);

    cube_print(C);
    cube_free(C);
}

int main()
{
    test_stacks();
    test_squares();
    test_cubes();

    printf("All tests passed!\n");
    return EXIT_SUCCESS;
}
