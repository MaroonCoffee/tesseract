#include <stdlib.h>
#include <stdio.h>
#include "cubes.h"
#include "squares.h"
#include "contracts.h"
#include "xalloc.h"

#define FILLER_CHAR '`'
#define EMPTY_CHAR ' '

struct cube_header {
    square_t left;
    square_t bottom;
    square_t front;
    square_t top;
    square_t right;
    square_t back;
    size_t length;
};

cube_t cube_new(size_t len)
/*requires len > 0*/
/*ensures result != NULL*/
{
    REQUIRES(len > 0);
    cube *C = xmalloc(sizeof(cube));
    
    C->left = NULL;
    C->bottom = NULL;
    C->front = NULL;
    C->top = NULL;
    C->right = NULL;
    C->back = NULL;
    C->length = len;

    cube *result = C;
    ENSURES(result != NULL);
    return result;
}

cube_t cube_initialize(cube_t C)
/*requires C != NULL*/
/*ensures result != NULL*/
{
    REQUIRES(C != NULL);
    size_t len = C->length;

    square_t left = square_new(len);
    square_t bottom = square_new(len);
    square_t front = square_new(len);
    square_t top = square_new(len);
    square_t right = square_new(len);
    square_t back = square_new(len);

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

square_t cube_read(cube_t C, size_t square)
/*requires C != NULL*/
/*requires square < 6*/
/*ensures result != NULL*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL);
    REQUIRES(square < 6);

    square_t result = NULL;
    switch(square) {
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

void cube_write(cube_t C, square_t S, size_t square)
/*requires C != NULL && S != NULL*/
/*requires square < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL && S != NULL);
    REQUIRES(square < 6);

    switch(square) {
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

size_t cube_length(cube_t C)
/*requires C != NULL*/
{
    REQUIRES(C != NULL);
    return C->length;
}

void print_three_squares(square_t s1, square_t s2, square_t s3)
/*requires square_length(s1) == square_length(s2)*/
/*requires square_length(s1) == square_length(s3)*/
{
    REQUIRES(square_length(s1) == square_length(s2));
    REQUIRES(square_length(s1) == square_length(s3));
    size_t len = square_length(s1);

    for (size_t i=0; i<(len+1)*3; i++){
        printf("%c", FILLER_CHAR);
    }
    printf("%c\n", FILLER_CHAR);

    for (size_t y=0; y<len; y++){
        printf("%c", FILLER_CHAR);
        for (size_t x=0; x<len; x++){
            char c = square_read(s1, x, y);
            if (c == 0) c = EMPTY_CHAR;
            printf("%c", c);
        }
        printf("%c", FILLER_CHAR);
        for (size_t x=0; x<len; x++){
            char c = square_read(s2, x, y);
            if (c == 0) c = EMPTY_CHAR;
            printf("%c", c);
        }
        printf("%c", FILLER_CHAR);
        for (size_t x=0; x<len; x++){
            char c = square_read(s3, x, y);
            if (c == 0) c = EMPTY_CHAR;
            printf("%c", c);
        }
        printf("%c\n", FILLER_CHAR);
    }
}

void cube_print(cube_t C)
/*requires C != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/
{
    REQUIRES(C != NULL);
    size_t len = cube_length(C);
    square_t dummy = square_new(len);

    for (size_t y=0; y<len; y++){
        for (size_t x=0; x<len; x++){
            square_write(dummy, FILLER_CHAR, x, y);
        }
    }
    
    print_three_squares(dummy, C->left, dummy);
    print_three_squares(C->bottom, C->front, C->top);
    print_three_squares(dummy, C->right, dummy);
    print_three_squares(dummy, C->back, dummy);

    for (size_t i=0; i<(len+1)*3; i++){
        printf("%c", FILLER_CHAR);
    }
    printf("%c\n", FILLER_CHAR);

    square_free(dummy);
}

void cube_free_face(cube_t C, size_t square)
/*requires C != NULL*/
/*requires square < 6*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(C != NULL);
    REQUIRES(square < 6);

    switch(square) {
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

void cube_free(cube_t C)
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
