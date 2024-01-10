#include <stdlib.h>
#include "contracts.h"
#include "xalloc.h"
#include "cursors.h"

struct cursor_header {
    size_t cube_index;
    size_t square_index;
    size_t x_index;
    size_t y_index;
    size_t dir;
};

cursor_t cursor_new()
/*ensures result != NULL*/
{
    cursor *C = xmalloc(sizeof(cursor));
    C->cube_index = 0;
    C->square_index = 0;
    C->x_index = 0;
    C->y_index = 0;
    C->dir = 0;

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

size_t cursor_get_dir(cursor_t C)
/*requires C != NULL*/
/*ensures result < 4*/
{
    REQUIRES(C != NULL);
    size_t dir = C->dir;
    size_t result = dir;
    ENSURES(result < 4);
    return result;
}

void cursor_set(cursor_t cursor, size_t C, size_t S, 
                size_t x, size_t y, size_t dir)
/*requires cursor != NULL*/
/*requires C < 8 && S < 6 && dir < 4*/
{
    REQUIRES(cursor != NULL);
    cursor->cube_index = C;
    cursor->square_index = S;
    cursor->x_index = x;
    cursor->y_index = y;
    cursor->dir = dir;
}

void cursor_dir_set(cursor_t cursor, size_t dir)
/*requries cursor != NULL*/
/*requires dir < 4*/
{
    REQUIRES(cursor != NULL);
    REQUIRES(dir < 4);
    cursor->dir = dir;
}

void cursor_free(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    free(C);
}
