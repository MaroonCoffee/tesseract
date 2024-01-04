#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stacks.h"
#include "contracts.h"
#include "xalloc.h"

typedef struct stacknode elem;
struct stacknode {
    genstack_elem *data;
    elem *next;
};

struct stackh {
    elem *next;
    size_t size;
};

genstack_t stack_new()
/*ensures result != NULL*/
{
    genstack *S = xmalloc(sizeof(genstack));
    S->next = NULL;
    S->size = 0;
    
    genstack *result = S;
    ENSURES(result != NULL);
    return result;
}

void stack_push(genstack_t S, genstack_elem *e)
/*requires S != NULL*/
/*ensures S != NULL && !stack_empty(S)*/
{
    REQUIRES(S != NULL);

    elem *eh = xmalloc(sizeof(elem));
    eh->data = e;
    eh->next = S->next;
    S->next = eh;
    S->size++;

    ENSURES(S != NULL && !stack_empty(S));
}

genstack_elem stack_pop(genstack_t S)
/*requires S != NULL && !stack_empty(S)*/
{
    REQUIRES(S != NULL && !stack_empty(S));

    elem *eh = S->next;
    S->next = eh->next;
    S->size--;
    genstack_elem *e = eh->data;
    free(eh);
    
    return e;
}

bool stack_empty(genstack_t S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    return S->size == 0;
}

size_t stack_size(genstack_t S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    return S->size;
}

void stack_print(genstack_t S, print_elem_fn *pr)
/*requires S != NULL && pr != NULL*/
{
    REQUIRES(S != NULL);
    printf("---TOP---\n");
    for (elem *C = S->next; C != NULL; C = C->next){
        (*pr)(C->data);
    }
    printf("--BOTTOM--\n");
}

void stack_free(genstack_t S, free_elem_fn *fr)
/*requires S != NULL*/
/*fr == NULL if the elements shouldn't be freed*/
{
    REQUIRES(S != NULL);
    elem *N = S->next;
    for (elem *C = N; C != NULL; C = N){
        if (fr != NULL)
            (*fr)(C->data);
        N = C->next;
        free(C);
    }
    free(S);
}
