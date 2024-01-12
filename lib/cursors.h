#include <stdlib.h>
#include <stdbool.h>

#ifndef CURSORS_H
#define CURSORS_H

typedef struct cursor_header cursor;
typedef cursor *cursor_t;

cursor_t cursor_new(size_t len);
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

char cursor_get_state(cursor_t C);
/*requires C != NULL*/

bool cursor_move_next(cursor_t C);
/*requires C != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
/*true is move caused a side switch*/

void cursor_wormhole(cursor_t C);
/*requires C != NULL*/
/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

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
