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

genstack_elem stack_peek_n(genstack_t S, size_t n)
/*requires S != NULL*/
/*requires n < stack_size(S)*/
{
    REQUIRES(S != NULL);
    REQUIRES(n < stack_size(S));
    elem *e = S->next;
    for (size_t i=0; i<n; i++){
        ASSERT(e != NULL);
        e = e->next;
    }
    ASSERT(e != NULL);
    return e->data;
}

genstack_elem stack_pop_n(genstack_t S, size_t n)
/*requires S != NULL*/
/*requires n < stack_size(S)*/
{
    REQUIRES(S != NULL);
    REQUIRES(n < stack_size(S));

    if (S->size == 1){
        return stack_pop(S);
    }

    elem *e = S->next;
    for (size_t i=0; i<n-1; i++){
        ASSERT(e != NULL);
        e = e->next;
    }
    ASSERT(e != NULL);
    
    elem *return_elem = e->next;
    ASSERT(return_elem != NULL);
    e->next = return_elem->next;

    genstack_elem *return_data = e->data;
    free(return_elem);
    return return_data;
}

void stack_swap_mn(genstack_t S, size_t m, size_t n)
/*requires S != NULL*/
/*requires stack_size(S) > 1*/
/*requires n < stack_size(S) && m < stack_size(S)*/
{
    REQUIRES(S != NULL);
    REQUIRES(n < stack_size(S) && m < stack_size(S));

    if (S->size == 1){
        return;
    }

    elem *m_ele = S->next;
    for (size_t i=0; i<m-1; i++){
        ASSERT(m_ele != NULL);
        m_ele = m_ele->next;
    }
    ASSERT(m_ele != NULL);

    elem *n_ele = S->next;
    for (size_t i=0; i<n-1; i++){
        ASSERT(n_ele != NULL);
        n_ele = n_ele->next;
    }
    ASSERT(n_ele != NULL);

    genstack_elem *temp = m_ele->data;
    m_ele->data = n_ele->data;
    n_ele->data = temp;
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
        if (fr != NULL){
            (*fr)(C->data);
        }
        N = C->next;
        free(C);
    }
    free(S);
}
