#include <stdlib.h>
#include "squares.h"

#ifndef CUBES_H
#define CUBES_H

typedef struct cube_header cube;

cube* cube_new(size_t len);
/*requires len > 0*/
/*ensures result != NULL*/

square* cube_read(cube* C, size_t face);
/*requires C != NULL*/
/*requires face < 6*/
/*ensures result != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

void cube_write(cube* C, square* S, size_t face);
/*requires C != NULL && S != NULL*/
/*requires face < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

size_t cube_length(cube *C);
/*requires C != NULL*/

void cube_print(cube* C);
/*requires C != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/

void cube_free_face(cube* C, size_t face);
/*requires C != NULL*/
/*requires face < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

void cube_free(cube* C);
/*requires C != NULL*/

#endif
