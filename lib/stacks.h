#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

typedef struct stackh genstack;
typedef void *genstack_elem;

typedef void print_elem_fn(genstack_elem e);
typedef void free_elem_fn(genstack_elem e);

genstack* stack_new();

void stack_push(genstack *S, genstack_elem *e);

genstack_elem stack_pop(genstack *S);

bool stack_empty(genstack *S);

int stack_size(genstack *S);

void stack_print(genstack *S, print_elem_fn *pr);

void stack_free(genstack *S, free_elem_fn *fr);

#endif
