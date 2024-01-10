#include <stdlib.h>
#include <stdbool.h>

#ifndef LOOPS_H
#define LOOPS_H

typedef struct loop_header loop;
typedef loop *loop_t;

typedef struct loop_dict_header loop_dict;
typedef loop_dict *loop_dict_t;

loop_dict_t loop_dict_new(size_t size);
/*requires size > 0*/
/*ensures result != NULL*/

loop_t loop_dict_lookup(loop_dict_t D, int key);
/*requires D != NULL*/
/*result is NULL if key does not exist in dict*/

void loop_init(loop_dict_t D, int key);
/*requires D != NULL*/

bool is_loop_active(loop_t L);
/*requires L != NULL*/

void loop_set(loop_t L, size_t len);
/*requires L != NULL*/
/*requires len > 0*/

void loop_decrease(loop_t L);
/*requires L != NULL*/
/*requires is_loop_active(L)*/

int loop_key(size_t C, size_t S, size_t x, size_t y, size_t len);
/*requires len > 0*/
/*requires C < 8 && S < 6 && x < len && y < len*/

void loop_dict_free(loop_dict_t D);
/*requires D != NULL*/

void loop_free(loop_t L);
/*requires L != NULL*/

#endif
