#include <stdlib.h>

#ifndef CURSORS_H
#define CURSORS_H

typedef struct cursor_header cursor;
typedef cursor *cursor_t;

cursor_t cursor_new();
/*ensures result != NULL*/

size_t cursor_get_cube(cursor_t C);
/*requires C != NULL*/

size_t cursor_get_square(cursor_t C);
/*requires C != NULL*/

size_t cursor_get_x(cursor_t C);
/*requires C != NULL*/

size_t cursor_get_y(cursor_t C);
/*requires C != NULL*/

void cursor_set_cube(cursor_t C, size_t cube);
/*requires C != NULL*/

void cursor_set_square(cursor_t C, size_t square);
/*requires C != NULL*/

void cursor_set_x(cursor_t C, size_t x);
/*requires C != NULL*/

void cursor_set_y(cursor_t C, size_t y);
/*requires C != NULL*/

void cursor_free(cursor_t C);
/*requires C != NULL*/

#endif
