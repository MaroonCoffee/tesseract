#include <stdlib.h>

#ifndef SQUARES_H
#define SQUARES_H

typedef struct square_header square;

square* square_new(size_t len);
/*requires len > 0*/
/*ensures result != NULL*/

size_t square_length(square* S);
/*requires S != NULL*/

char square_read(square* S, size_t x, size_t y);
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/

void square_write(square* S, char c, size_t x, size_t y);
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/

void square_print(square* S);
/*requires S != NULL*/
/*empty characters displayed as `*/

void square_free(square* S);
/*requires S != NULL*/

#endif
