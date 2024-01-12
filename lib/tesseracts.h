#include <stdlib.h>
#include "cubes.h"
#include "cursors.h"
#include "loops.h"

#ifndef TESSERACTS_H
#define TESSERACTS_H

typedef struct tesseract_header tesseract;
typedef tesseract *tesseract_t;

tesseract_t tesseract_new(size_t len);
/*requires len > 0*/
/*ensures result != NULL*/

tesseract_t tesseract_initialize(tesseract_t T);
/*requires T != NULL*/
/*ensures result != NULL*/

void tesseract_initial_parse(tesseract_t T);
/*requires T != NULL*/

loop_dict_t tesseract_get_loop_dict(tesseract_t T);
/*requires T != NULL*/
/*ensures result != NULL*/

cube_t tesseract_read(tesseract_t T, size_t cube);
/*requires T != NULL*/
/*requires cube < 8*/
/*ensures result != NULL*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

square_t tesseract_square_read(tesseract_t T, size_t cube, size_t square);
/*requires T != NULL*/
/*requires cube < 8*/
/*requires square < 6*/
/*ensures result != NULL*/

/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

char tesseract_cell_read(tesseract_t T, size_t C, size_t S, size_t x, size_t y);
/*requires T != NULL*/
/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

char tesseract_cursor_read(tesseract_t T);
/*requires T != NULL*/

void tesseract_write(tesseract_t T, cube_t C, size_t cube);
/*requires T != NULL && C != NULL*/
/*requires cube < 8*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

void tesseract_cell_write(tesseract_t T, size_t C, size_t S, 
                          size_t x, size_t y, char symbol);
/*requires T != NULL*/
/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

size_t tesseract_length(tesseract_t T);
/*requires T != NULL*/

cursor_t tesseract_cursor(tesseract_t T);
/*requires T != NULL*/

void tesseract_print(tesseract_t T, bool print_cursor);
/*requires T != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/

/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

/*comments above each print_tesseract_line call display what an empty*/
/*length 1 tesseract should display after the call*/

void tesseract_free_cube(tesseract_t T, size_t cube);
/*requires C != NULL*/
/*requires cube < 8*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

void tesseract_free(tesseract_t T);
/*requires T != NULL*/

#endif
