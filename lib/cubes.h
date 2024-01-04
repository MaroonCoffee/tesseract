#include <stdlib.h>
#include "squares.h"

#ifndef CUBES_H
#define CUBES_H

typedef struct cube_header cube;
typedef cube *cube_t;

cube_t cube_new(size_t len);
/*requires len > 0*/
/*ensures result != NULL*/

square_t cube_read(cube_t C, size_t square);
/*requires C != NULL*/
/*requires square < 6*/
/*ensures result != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

void cube_write(cube_t C, square_t S, size_t square);
/*requires C != NULL && S != NULL*/
/*requires square < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

size_t cube_length(cube_t C);
/*requires C != NULL*/

void cube_print(cube_t C);
/*requires C != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/

void cube_free_face(cube_t C, size_t square);
/*requires C != NULL*/
/*requires square < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

void cube_free(cube_t C);
/*requires C != NULL*/

#endif
