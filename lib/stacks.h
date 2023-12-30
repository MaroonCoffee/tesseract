#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

typedef struct stackh genstack;
typedef void *genstack_elem;

typedef void print_elem_fn(genstack_elem e);
typedef void free_elem_fn(genstack_elem e);

genstack* stack_new();
/*ensures result != NULL*/

void stack_push(genstack *S, genstack_elem *e);
/*requires S != NULL*/
/*ensures S != NULL && !stack_empty(S)*/

genstack_elem stack_pop(genstack *S);
/*requires S != NULL && !stack_empty(S)*/

bool stack_empty(genstack *S);
/*requires S != NULL*/

size_t stack_size(genstack *S);
/*requires S != NULL*/

void stack_print(genstack *S, print_elem_fn *pr);
/*requires S != NULL && pr != NULL*/

void stack_free(genstack *S, free_elem_fn *fr);
/*requires S != NULL*/
/*fr == NULL if the elements shouldn't be freed*/

#endif
