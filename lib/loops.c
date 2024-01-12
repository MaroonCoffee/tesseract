#include <stdlib.h>
#include <stdbool.h>
#include "loops.h"
#include "contracts.h"
#include "xalloc.h"
#include "stacks.h"

struct loop_header {
    size_t state;
    int key;
    loop_t next;
};

struct loop_dict_header {
    size_t size;
    loop_t *data;
};

loop_dict_t loop_dict_new(size_t size)
/*requires size > 0*/
/*ensures result != NULL*/
{
    REQUIRES(size > 0);
    loop_dict *D = xmalloc(sizeof(loop_dict));
    D->size = size;
    D->data = xcalloc(size, sizeof(loop_t));
    
    loop_dict_t result = D;
    ENSURES(result != NULL);
    return result;
}

loop_t loop_dict_lookup(loop_dict_t D, int key)
/*requires D != NULL*/
/*result is NULL if key does not exist in dict*/
{
    REQUIRES(D != NULL);
    size_t index = key%D->size;
    for (loop_t chain = D->data[index]; chain != NULL; chain = chain->next){
        if (chain->key == key){
            return chain;
        }
    }
    return NULL;
}

void loop_init(loop_dict_t D, int key)
/*requires D != NULL*/
{
    REQUIRES(D != NULL);
    loop *L = xmalloc(sizeof(loop));
    L->key = key;
    L->state = 0;
    
    size_t index = key%D->size;
    L->next = D->data[index];
    D->data[index] = L;
}

bool is_loop_active(loop_t L)
/*requires L != NULL*/
{
    REQUIRES(L != NULL);
    return L->state > 0;
}

void loop_set(loop_t L, size_t len)
/*requires L != NULL*/
/*requires len > 0*/
{
    REQUIRES(L != NULL);
    REQUIRES(len > 0);
    L->state = len;
}

void loop_decrease(loop_t L)
/*requires L != NULL*/
/*requires is_loop_active(L)*/
{
    REQUIRES(L != NULL);
    REQUIRES(is_loop_active(L));
    L->state = L->state - 1;
}

int loop_key(size_t C, size_t S, size_t x, size_t y, size_t len)
/*requires len > 0*/
/*requires C < 8 && S < 6 && x < len && y < len*/
{
    REQUIRES(len > 0);
    REQUIRES(C < 8 && S < 6 && x < len && y < len);

    int key = C*6*len*len + S*len*len + y*len + x;
    return 1664525*key + 1013904223;
}

void loop_dict_free(loop_dict_t D)
/*requires D != NULL*/
{
    REQUIRES(D != NULL);
    for (size_t i=0; i<D->size; i++){
        loop_t chain = D->data[i];
        while (chain != NULL){
            loop_t next = chain->next;
            loop_free(chain);
            chain = next;
        }
    }
    free(D);
}

void loop_dict_print(loop_dict_t D, size_t len)
/*requires D != NULL*/
/*requires len > 0*/
{
    REQUIRES(D != NULL);
    for (size_t i=0; i<D->size; i++){
        loop *L = D->data[i];
        while (L != NULL){
            int key = (L->key - 1013904223) / 1664525;
            int temp = key;
            int C = temp/(6*len*len);
            temp = temp%(6*len*len);
            int S = temp/(len*len);
            temp = temp%(len*len);
            int y = temp/len;
            temp = temp%len;
            int x = temp;

            printf("State: %zu, Chain: %zu, Cube: %d, Square: %d, x: %d, y: %d",
                  L->state, key%D->size, C, S, x, y);
            printf("\n");
            
            L = L->next;
        }
    }
}

void loop_free(loop_t L)
/*requires L != NULL*/
{
    REQUIRES (L != NULL);
    free(L);
}
