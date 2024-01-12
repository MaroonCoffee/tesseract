#include <stdbool.h>
#include <stdlib.h>

#ifndef STACK_H
#define STACK_H

typedef struct stackh genstack;
typedef void *genstack_elem;
typedef genstack *genstack_t;

typedef void print_elem_fn(genstack_elem e);
typedef void free_elem_fn(genstack_elem e);

genstack_t stack_new();
/*ensures result != NULL*/

void stack_push(genstack_t S, genstack_elem *e);
/*requires S != NULL*/
/*ensures S != NULL && !stack_empty(S)*/

genstack_elem stack_pop(genstack_t S);
/*requires S != NULL && !stack_empty(S)*/

genstack_elem stack_peek_n(genstack_t S, size_t n);
/*requires S != NULL*/
/*requires n < stack_size(S)*/

genstack_elem stack_pop_n(genstack_t S, size_t n);
/*requires S != NULL*/
/*requires n < stack_size(S)*/

void stack_swap_mn(genstack_t S, size_t m, size_t n);
/*requires S != NULL*/
/*requires n < stack_size(S) && m < stack_size(S)*/

bool stack_empty(genstack_t S);
/*requires S != NULL*/

size_t stack_size(genstack_t S);
/*requires S != NULL*/

void stack_print(genstack_t S, print_elem_fn *pr);
/*requires S != NULL && pr != NULL*/

void stack_free(genstack_t S, free_elem_fn *fr);
/*requires S != NULL*/
/*fr == NULL if the elements shouldn't be freed*/

#endif
