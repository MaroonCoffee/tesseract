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

size_t cursor_get_dir(cursor_t C);
/*requires C != NULL*/
/*ensures result < 4*/

void cursor_set(cursor_t cursor, size_t C, size_t S, 
                size_t x, size_t y, size_t dir);
/*requires cursor != NULL*/
/*requires C < 8 && S < 6 && dir < 4*/

void cursor_dir_set(cursor_t cursor, size_t dir);
/*requries cursor != NULL*/
/*requires dir < 4*/

void cursor_free(cursor_t C);
/*requires C != NULL*/

#endif
