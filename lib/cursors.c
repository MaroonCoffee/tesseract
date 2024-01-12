#include <stdlib.h>
#include <stdbool.h>
#include "contracts.h"
#include "xalloc.h"
#include "cursors.h"

struct cursor_header {
    size_t cube_index;
    size_t square_index;
    size_t x_index;
    size_t y_index;
    size_t dir;
    size_t len;
};

cursor_t cursor_new(size_t len)
/*ensures result != NULL*/
{
    cursor *C = xmalloc(sizeof(cursor));
    C->cube_index = 0;
    C->square_index = 0;
    C->x_index = 0;
    C->y_index = 0;
    C->dir = 0;
    C->len = len;

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

char cursor_get_state(cursor_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    switch(C->dir) {
        case 0:
            return '^';
        case 1:
            return '>';
        case 2:
            return 'v';
        case 3:
            return '<';
        default:
            //this should never execute
            ASSERT(false);
            return '\0';
    }
}

bool cursor_move_next(cursor_t C)
/*requires C != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
/*true is move caused a side switch*/
{
    REQUIRES (C != NULL);
    size_t x = C->x_index;
    size_t y = C->y_index;
    size_t len = C->len;

    bool no_side_switch = true;
    switch(C->dir) {
        case 0:
            if (y > 0) C->y_index = y - 1;
            else no_side_switch = false;
            break;
        case 1:
            if (x < len-1) C->x_index = x + 1;
            else no_side_switch = false;
            break;
        case 2:
            if (y < len-1) C->y_index = y + 1;
            else no_side_switch = false;
            break;
        case 3:
            if (x > 0) C->x_index = x - 1;
            else no_side_switch = false;
            break;
    }
    if (no_side_switch) return false;

    switch(C->square_index) {
        case 0:
            switch(C->dir) {
                case 0: 
                    //Left (0) to Back (5)
                    C->square_index = 5;
                    C->y_index = len-1;
                    return true;
                case 1:
                    //Left (0) to Top (3)
                    C->square_index = 3;
                    C->x_index = len-1-y;
                    C->y_index = 0;
                    C->dir = 2;
                    return true;
                case 2:
                    //Left (0) to Front (2)
                    C->square_index = 2;
                    C->y_index = 0;
                    return true;
                case 3:
                    //Left (0) to Bottom (1)
                    C->square_index = 1;
                    C->x_index = y;
                    C->y_index = 0;
                    C->dir = 2;
                    return true;
            }
            break;
            
        case 1:
            switch(C->dir) {
                case 0:
                    //Bottom (1) to Left (0)
                    C->square_index = 0;
                    C->x_index = 0;
                    C->y_index = x;
                    C->dir = 1;
                    return true;
                case 1:
                    //Bottom (1) to Front (2)
                    C->square_index = 2;
                    C->x_index = 0;
                    return true;
                case 2:
                    //Bottom (1) to Right (4)
                    C->square_index = 4;
                    C->x_index = 0;
                    C->y_index = len-1-x;
                    C->dir = 1;
                    return true;
                case 3:
                    //Bottom (1) to Back (5)
                    C->square_index = 5;
                    C->y_index = len-1-y;
                    C->dir = 1;
                    return true;
            }
            break;

        case 2:
            switch(C->dir) {
                case 0:
                    //Front (2) to Left (0)
                    C->square_index = 0;
                    C->y_index = len-1;
                    return true;
                case 1:
                    //Front (2) to Top (3)
                    C->square_index = 3;
                    C->x_index = 0;
                    return true;
                case 2:
                    //Front (2) to Right (4)
                    C->square_index = 4;
                    C->y_index = 0;
                    return true;
                case 3:
                    //Front (2) to Bottom (1)
                    C->square_index = 1;
                    C->x_index = len-1;
                    return true;
            }
            break;

        case 3:
            switch(C->dir) {
                case 0:
                    //Top (3) to Left (0)
                    C->square_index = 0;
                    C->x_index = len-1;
                    C->y_index = len-1-x;
                    C->dir = 3;
                    return true;
                case 1:
                    //Top (3) to Back (5)
                    C->square_index = 5;
                    C->y_index = len-1-y;
                    C->dir = 3;
                    return true;
                case 2:
                    //Top (3) to Right (4)
                    C->square_index = 4;
                    C->x_index = len-1;
                    C->y_index = x;
                    C->dir = 3;
                    return true;
                case 3:
                    //Top (3) to Front (2)
                    C->square_index = 2;
                    C->x_index = len-1;
                    return true;
            }
            break;

        case 4:
            switch(C->dir) {
                case 0:
                    //Right (4) to Front (2)
                    C->square_index = 2;
                    C->y_index = len-1;
                    return true;
                case 1:
                    //Right (4) to Top (3)
                    C->square_index = 3;
                    C->x_index = y;
                    C->y_index = len-1;
                    C->dir = 0;
                    return true;
                case 2:
                    //Right (4) to Back (5)
                    C->square_index = 5;
                    C->y_index = 0;
                    return true;
                case 3:
                    //Right (4) to Bottom (1)
                    C->square_index = 1;
                    C->x_index = len-1-y;
                    C->y_index = len-1;
                    C->dir = 0;
                    return true;
            }
            break;

        case 5:
            switch(C->dir) {
                case 0:
                    //Back (5) to Right (4)
                    C->square_index = 4;
                    C->y_index = len-1;
                    return true;
                case 1:
                    //Back (5) to Top (3)
                    C->square_index = 3;
                    C->y_index = len-1-y;
                    C->dir = 3;
                    return true;
                case 2:
                    //Back (5) to Left (0)
                    C->square_index = 0;
                    C->y_index = 0;
                    return true;
                case 3:
                    //Back (5) to Bottom (1)
                    C->square_index = 1;
                    C->y_index = len-1-y;
                    C->dir = 1;
                    return true;
            }
            break;
    }
    //this should never execute
    ASSERT(false);
    return true;
}

void cursor_wormhole(cursor_t C)
/*requires C != NULL*/
/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL);
    switch(C->cube_index) {
        case 0:
            switch(C->square_index) {
                case 0:
                    //Top (0) Left (0) to Left (5) Top (3)
                    C->cube_index = 5;
                    C->square_index = 3;
                    return;
                case 1:
                    //Top (0) Bottom (1) to Center (1) Top (3)
                    C->cube_index = 1;
                    C->square_index = 3;
                    return;
                case 2:
                    //Top (0) Front (2) to Front (3) Top (3)
                    C->cube_index = 3;
                    C->square_index = 3;
                    return;
                case 3:
                    //Top (0) Top (3) to Rightmost (7) Top (3)
                    C->cube_index = 7;
                    C->square_index = 3;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 4:
                    //Top (0) Right (4) to Right (6) Top (3)
                    C->cube_index = 6;
                    C->square_index = 3;
                    return;
                case 5:
                    //Top (0) Back (5) to Back (4) Top (3)
                    C->cube_index = 4;
                    C->square_index = 3;
                    return;
            }
            break;

        case 1:
            switch(C->square_index) {
                case 0:
                    //Center (1) Left (0) to Left (5) Right (4)
                    C->cube_index = 5;
                    C->square_index = 4;
                    return;
                case 1:
                    //Center (1) Bottom (1) to Bottom (2) Top (3)
                    C->cube_index = 2;
                    C->square_index = 3;
                    return;
                case 2:
                    //Center (1) Front (2) to Front (3) Back (5)
                    C->cube_index = 3;
                    C->square_index = 5;
                    return;
                case 3:
                    //Center (1) Top (3) to Top (0) Left (1)
                    C->cube_index = 0;
                    C->square_index = 1;
                    return;
                case 4:
                    //Center (1) Right (4) to Right (4) Left (0)
                    C->cube_index = 4;
                    C->square_index = 0;
                    return;
                case 5:
                    //Center (1) Back (5) to Back (4) Front (2)
                    C->cube_index = 4;
                    C->square_index = 2;
                    return;
            }
            break;

            case 2:
                switch(C->square_index) {
                case 0:
                    //Bottom (2) Left (0) to Left (5) Bottom (1)
                    C->cube_index = 5;
                    C->square_index = 1;
                    return;
                case 1:
                    //Bottom (2) Bottom (1) to Rightmost (7) Bottom (1)
                    C->cube_index = 7;
                    C->square_index = 1;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 2:
                    //Bottom (2) Front (2) to Front (3) Bottom (1)
                    C->cube_index = 3;
                    C->square_index = 1;
                    return;
                case 3:
                    //Bottom (2) Top (3) to Center (1) Bottom (1)
                    C->cube_index = 1;
                    C->square_index = 1;
                    return;
                case 4:
                    //Bottom (2) Right (4) to Right (6) Bottom (1)
                    C->cube_index = 6;
                    C->square_index = 1;
                    return;
                case 5:
                    //Bottom (2) Back (5) to Back (4) Bottom (1)
                    C->cube_index = 4;
                    C->square_index = 1;
                    return;
            }
            break;

            case 3:
                switch(C->square_index) {
                case 0:
                    //Front (3) Left (0) to Left (5) Front (2)
                    C->cube_index = 5;
                    C->square_index = 2;
                    return;
                case 1:
                    //Front (3) Bottom (1) to Bottom (2) Front (2)
                    C->cube_index = 2;
                    C->square_index = 2;
                    return;
                case 2:
                    //Front (3) Front (2) to Rightmost (7) Front (2)
                    C->cube_index = 7;
                    C->square_index = 2;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 3:
                    //Front (3) Top (3) to Top (0) Front (2)
                    C->cube_index = 0;
                    C->square_index = 2;
                    return;
                case 4:
                    //Front (3) Right (4) to Right (6) Front (2)
                    C->cube_index = 6;
                    C->square_index = 2;
                    return;
                case 5:
                    //Front (3) Back (5) to Center (1) Front (2)
                    C->cube_index = 1;
                    C->square_index = 2;
                    return;
            }
            break;

            case 4:
                switch(C->square_index) {
                case 0:
                    //Back (4) Left (0) to Left (5) Back (5)
                    C->cube_index = 5;
                    C->square_index = 5;
                    return;
                case 1:
                    //Back (4) Bottom (1) to Bottom (2) Back (5)
                    C->cube_index = 2;
                    C->square_index = 5;
                    return;
                case 2:
                    //Back (4) Front (2) to Center (1) Back (5)
                    C->cube_index = 1;
                    C->square_index = 5;
                    return;
                case 3:
                    //Back (4) Top (3) to Left (0) Back (5)
                    C->cube_index = 0;
                    C->square_index = 5;
                    return;
                case 4:
                    //Back (4) Right (4) to Right (6) Back (5)
                    C->cube_index = 6;
                    C->square_index = 5;
                    return;
                case 5:
                    //Back (4) Back (5) to Rightmost (7) Back (5)
                    C->cube_index = 7;
                    C->square_index = 5;
                    C->dir = (C->dir + 2) % 4;
                    return;
            }
            break;

            case 5:
                switch(C->square_index) {
                case 0:
                    //Left (5) Left (0) to Rightmost (7) Right (4)
                    C->cube_index = 7;
                    C->square_index = 4;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 1:
                    //Left (5) Bottom (1) to Bottom (2) Left (0)
                    C->cube_index = 2;
                    C->square_index = 0;
                    return;
                case 2:
                    //Left (5) Front (2) to Front (3) Left (0)
                    C->cube_index = 3;
                    C->square_index = 0;
                    return;
                case 3:
                    //Left (5) Top (3) to Top (0) Left (0)
                    C->cube_index = 0;
                    C->square_index = 0;
                    return;
                case 4:
                    //Left (5) Right (4) to Center (1) Left (0)
                    C->cube_index = 1;
                    C->square_index = 0;
                    return;
                case 5:
                    //Left (5) Back (5) to Back (4) Left (0)
                    C->cube_index = 4;
                    C->square_index = 0;
                    return;
            }
            break;

            case 6:
                switch(C->square_index) {
                case 0:
                    //Right (6) Left (0) to Center (1) Right (4)
                    C->cube_index = 1;
                    C->square_index = 4;
                    return;
                case 1:
                    //Right (6) Bottom (1) to Bottom (2) Right (4)
                    C->cube_index = 2;
                    C->square_index = 4;
                    return;
                case 2:
                    //Right (6) Front (2) to Front (2) Right (4)
                    C->cube_index = 2;
                    C->square_index = 4;
                    return;
                case 3:
                    //Right (6) Top (3) to Top (0) Right (4)
                    C->cube_index = 0;
                    C->square_index = 4;
                    return;
                case 4:
                    //Right (6) Right (4) to Rightmost (7) Left (0)
                    C->cube_index = 7;
                    C->square_index = 0;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 5:
                    //Right (6) Back (5) to Back (4) Right (4)
                    C->cube_index = 4;
                    C->square_index = 4;
                    return;
            }
            break;

            /*tesseract indices*/
            /*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
            /*cube indices*/
            /*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
            case 7:
                switch(C->square_index) {
                case 0:
                    //Rightmost (7) Left (0) to Right (6) Right (4)
                    C->cube_index = 6;
                    C->square_index = 4;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 1:
                    //Rightmost (7) Bottom (1) to Bottom (2) Bottom (1)
                    C->cube_index = 2;
                    C->square_index = 1;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 2:
                    //Rightmost (7) Front (2) to Front (3) Front (2)
                    C->cube_index = 3;
                    C->square_index = 2;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 3:
                    //Rightmost (7) Top (3) to Top (0) Top (3)
                    C->cube_index = 0;
                    C->square_index = 3;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 4:
                    //Rightmost (7) Right (4) to Left (5) Left (0)
                    C->cube_index = 5;
                    C->square_index = 0;
                    C->dir = (C->dir + 2) % 4;
                    return;
                case 5:
                    //Rightmost (7) Back (5) to Back (4) Back (5)
                    C->cube_index = 4;
                    C->square_index = 5;
                    C->dir = (C->dir + 2) % 4;
                    return;
            }
            break;
    }
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
