#include <stdlib.h>
#include "contracts.h"
#include "xalloc.h"
#include "cursors.h"

struct cursor_header {
    size_t cube_index;
    size_t square_index;
    size_t x_index;
    size_t y_index;
};

cursor_t cursor_new()
/*ensures result != NULL*/
{
    cursor *C = xmalloc(sizeof(cursor));
    C->cube_index = 0;
    C->square_index = 0;
    C->x_index = 0;
    C->y_index = 0;

    cursor *result = C;
    ENSURES(result != NULL);
    return result;
}

size_t cursor_get_cube(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    return C->cube_index;
}

size_t cursor_get_square(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    return C->square_index;
}

size_t cursor_get_x(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    return C->x_index;
}

size_t cursor_get_y(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    return C->y_index;
}

void cursor_set_cube(cursor_t C, size_t cube)
/*requires C != NULL*/
/*requires cube < 8*/
{
    REQUIRES(C != NULL);
    REQUIRES(cube < 8);
    C->cube_index = cube;
}

void cursor_set_square(cursor_t C, size_t square)
/*requires C != NULL*/
/*requires square < 6*/
{
    REQUIRES(C != NULL);
    REQUIRES(square < 6);
    C->square_index = square;
}

void cursor_set_x(cursor_t C, size_t x)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    C->x_index = x;
}

void cursor_set_y(cursor_t C, size_t y)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    C->y_index = y;
}

void cursor_free(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    free(C);
}
