#include <stdlib.h>
#include "cubes.h"
#include "squares.h"
#include "contracts.h"
#include "xalloc.h"

struct cube_header {
    square *left;
    square *bottom;
    square *front;
    square *top;
    square *right;
    square *back;
    size_t length;
};

cube* cube_new(size_t len)
/*requires len > 0*/
/*ensures result != NULL*/
{
    REQUIRES(len > 0);
    cube *C = xmalloc(sizeof(cube));
    square *left = square_new(len);
    square *bottom = square_new(len);
    square *front = square_new(len);
    square *top = square_new(len);
    square *right = square_new(len);
    square *back = square_new(len);

    C->left = left;
    C->bottom = bottom;
    C->front = front;
    C->top = top;
    C->right = right;
    C->back = back;
    C->length = len;

    cube *result = C;
    ENSURES(result != NULL);
    return result;
}

square* cube_read(cube* C, size_t face)
/*requires C != NULL*/
/*requires face < 6*/
/*ensures result != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL);
    REQUIRES(face < 6);

    square *result = NULL;
    switch(face) {
        case 0:
            result = C->left;
            break;
        case 1:
            result = C->bottom;
            break;
        case 2:
            result = C->front;
            break;
        case 3:
            result = C->top;
            break;
        case 4:
            result = C->right;
            break;
        case 5:
            result = C->back;
            break;
    }

    ENSURES(result != NULL);
    return result;
}

void cube_write(cube* C, square* S, size_t face)
/*requires C != NULL && S != NULL*/
/*requires face < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL && S != NULL);
    REQUIRES(face < 6);

    switch(face) {
        case 0:
            C->left = S;
            break;
        case 1:
            C->bottom = S;
            break;
        case 2:
            C->front = S;
            break;
        case 3:
            C->top = S;
            break;
        case 4:
            C->right = S;
            break;
        case 5:
            C->back = S;
            break;
    }
}

size_t cube_length(cube *C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    size_t result = C->length;
    return result;
}

void print_three_squares(square* s1, square* s2, square* s3)
/*requires square_length(s1) == square_length(s2)*/
/*requires square_length(s1) == square_length(s3)*/
{
    REQUIRES(square_length(s1) == square_length(s2));
    REQUIRES(square_length(s1) == square_length(s3));
    size_t len = square_length(s1);

    for (size_t i=0; i<(len+1)*3; i++){
        printf("%c", '`');
    }
    printf("%c\n", '`');

    for (size_t y=0; y<len; y++){
        printf("%c", '`');
        for (size_t x=0; x<len; x++){
            char c = square_read(s1, x, y);
            if (c == 0) c = 'o';
            printf("%c", c);
        }
        printf("%c", '`');
        for (size_t x=0; x<len; x++){
            char c = square_read(s2, x, y);
            if (c == 0) c = 'o';
            printf("%c", c);
        }
        printf("%c", '`');
        for (size_t x=0; x<len; x++){
            char c = square_read(s3, x, y);
            if (c == 0) c = 'o';
            printf("%c", c);
        }
        printf("%c\n", '`');
    }
}

void cube_print(cube* C)
/*requires C != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/
{
    REQUIRES(C != NULL);
    size_t len = cube_length(C);
    square *dummy = square_new(len);

    for (size_t y=0; y<len; y++){
        for (size_t x=0; x<len; x++){
            square_write(dummy, '`', x, y);
        }
    }
    
    print_three_squares(dummy, C->left, dummy);
    print_three_squares(C->bottom, C->front, C->top);
    print_three_squares(dummy, C->right, dummy);
    print_three_squares(dummy, C->back, dummy);

    for (size_t i=0; i<(len+1)*3; i++){
        printf("%c", '`');
    }
    printf("%c\n", '`');

    square_free(dummy);
}

void cube_free_face(cube* C, size_t face)
/*requires C != NULL*/
/*requires face < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL);
    REQUIRES(face < 6);

    switch(face) {
        case 0:
            square_free(C->left);
            break;
        case 1:
            square_free(C->bottom);
            break;
        case 2:
            square_free(C->front);
            break;
        case 3:
            square_free(C->top);
            break;
        case 4:
            square_free(C->right);
            break;
        case 5:
            square_free(C->back);
            break;
    }
}

void cube_free(cube* C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    square_free(C->left);
    square_free(C->front);
    square_free(C->bottom);
    square_free(C->top);
    square_free(C->right);
    square_free(C->back);
    free(C);
}
