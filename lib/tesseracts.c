#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "contracts.h"
#include "xalloc.h"
#include "cubes.h"
#include "tesseracts.h"

struct tesseract_header {
    cube_t top;
    cube_t center;
    cube_t bottom;
    cube_t front;
    cube_t back;
    cube_t left;
    cube_t right;
    cube_t rightmost;
    size_t length;
};

tesseract_t tesseract_new(size_t len)
/*requires len > 0*/
/*ensures result != NULL*/
{
    REQUIRES(len > 0);
    tesseract *T = xmalloc(sizeof(tesseract));
    cube_t top = cube_new(len);
    cube_t center = cube_new(len);
    cube_t bottom = cube_new(len);
    cube_t front = cube_new(len);
    cube_t back = cube_new(len);
    cube_t left = cube_new(len);
    cube_t right = cube_new(len);
    cube_t rightmost = cube_new(len);

    T->top = top;
    T->center = center;
    T->bottom = bottom;
    T->front = front;
    T->back = back;
    T->left = left;
    T->right = right;
    T->rightmost = rightmost;
    T->length = len;

    tesseract *result = T;
    ENSURES(result != NULL);
    return result;
}

cube_t tesseract_read(tesseract_t T, size_t cube_face)
/*requires T != NULL*/
/*requires cube_face < 8*/
/*ensures result != NULL*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
{
    REQUIRES(T != NULL);
    REQUIRES(cube_face < 8);

    cube_t result = NULL;
    switch(cube_face) {
        case 0:
            result = T->top;
            break;
        case 1:
            result = T->center;
            break;
        case 2:
            result = T->bottom;
            break;
        case 3:
            result = T->front;
            break;
        case 4:
            result = T->back;
            break;
        case 5:
            result = T->left;
            break;
        case 6:
            result = T->right;
            break;
        case 7:
            result = T->rightmost;
            break;
    }

    ENSURES(result != NULL);
    return result;
}

square_t tesseract_square_read(tesseract_t T, size_t cube, size_t square)
/*requires T != NULL*/
/*requires cube < 8*/
/*requires square < 6*/
/*ensures result != NULL*/

/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(T != NULL);
    REQUIRES(cube < 8);
    REQUIRES(square < 6);
    
    square_t result = cube_read(tesseract_read(T, cube), square);
    ENSURES(result != NULL);
    return result;
}

void tesseract_write(tesseract_t T, cube_t C, size_t cube_face)
/*requires T != NULL && C != NULL*/
/*requires cube_face < 8*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
{
    REQUIRES(T != NULL && C != NULL);
    REQUIRES(cube_face < 8);

    switch(cube_face) {
        case 0:
            T->top = C;
            break;
        case 1:
            T->center = C;
            break;
        case 2:
            T->bottom = C;
            break;
        case 3:
            T->front = C;
            break;
        case 4:
            T->back = C;
            break;
        case 5:
            T->left = C;
            break;
        case 6:
            T->right = C;
            break;
        case 7:
            T->rightmost = C;
            break;
    }
}

size_t tesseract_length(tesseract_t T)
/*requires T != NULL*/
{
    REQUIRES(T != NULL);
    return T->length;
}

void print_n_ticks(size_t n)
{
    for (size_t i=0; i<n; i++){
        printf("`");
    }
}

bool all_squares_non_null(square_t squares[], size_t squares_len) 
{
    for (size_t i=0; i<squares_len; i++)
        if (squares[i] == NULL)
            return false;
    return true;
}

bool all_squares_same_length(square_t squares[], size_t squares_len)
{
    for (size_t i=1; i<squares_len; i++) 
        if (square_length(squares[i]) != square_length(squares[0]))
            return false;
    return true;
}

void print_tesseract_line(square_t squares[], size_t spaces[], size_t array_len)
/*requires array_len > 0*/
/*requires all_squares_non_null(squares, array_len)*/
/*requires all_squares_same_length(squares, array_len)*/
/*requires \length(squares) == array_len && \length(spaces) == array_len*/
{
    REQUIRES(array_len > 0);
    REQUIRES(all_squares_non_null(squares, array_len));
    REQUIRES(all_squares_same_length(squares, array_len));

    int remaining_space = 22-array_len;
    for (size_t i=0; i<array_len; i++)
        remaining_space -= spaces[i];
    ASSERT(remaining_space >= 0);
    
    size_t tesseract_len = square_length(squares[0]);
    for (size_t y=0; y<tesseract_len; y++){
        printf("/*");
        for (size_t square=0; square<array_len; square++){
            print_n_ticks(spaces[square]*tesseract_len);
            for (size_t x=0; x<tesseract_len; x++){
                char c = square_read(squares[square], x, y);
                if (c == 0) c = 'o';
                printf("%c", c);
            }
        }
        print_n_ticks(remaining_space*tesseract_len);
        printf("*/\n");
    }
    printf("/*");
    print_n_ticks(tesseract_len*22);
    printf("*/\n");
}

void tesseract_print(tesseract_t T)
/*requires T != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/

/*tesseract indices*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/

/*cube indices*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/

/*comments above each print_tesseract_line call display what an empty*/
/*length 1 tesseract should display after the call (with the following*/
/*empty line removed to avoid redundancy)*/

{
    REQUIRES(T != NULL);
    size_t len = T->length;
    
    /*n.....................*/
    printf("/*%zu", len);
    print_n_ticks(len*22 - 1);
    printf("*/\n");

    /*........o.............*/
    square_t squares_1[] = {tesseract_square_read(T, 0, 0)};
    size_t spaces_1[] = {8};
    print_tesseract_line(squares_1, spaces_1, 1);

    /*......o.o.o...........*/
    square_t squares_2[] = {tesseract_square_read(T, 0, 1), 
                            tesseract_square_read(T, 0, 2),
                            tesseract_square_read(T, 0, 3)};
    size_t spaces_2[] = {6, 1, 1};
    print_tesseract_line(squares_2, spaces_2, 3);

    /*........o.............*/
    square_t squares_3[] = {tesseract_square_read(T, 0, 4)};
    size_t spaces_3[] = {8};
    print_tesseract_line(squares_3, spaces_3, 1);

    /*........o.............*/
    square_t squares_4[] = {tesseract_square_read(T, 0, 5)};
    size_t spaces_4[] = {8};
    print_tesseract_line(squares_4, spaces_4, 1);

    /*...o.........o........*/
    square_t squares_5[] = {tesseract_square_read(T, 5, 0),
                            tesseract_square_read(T, 4, 0)};
    size_t spaces_5[] = {3, 9};
    print_tesseract_line(squares_5, spaces_5, 2);

    /*.o.o.o.....o.o.o......*/
    square_t squares_6[] = {tesseract_square_read(T, 5, 1),
                            tesseract_square_read(T, 5, 2),
                            tesseract_square_read(T, 5, 3),
                            tesseract_square_read(T, 4, 1),
                            tesseract_square_read(T, 4, 2),
                            tesseract_square_read(T, 4, 3)};
    size_t spaces_6[] = {1, 1, 1, 5, 1, 1};
    print_tesseract_line(squares_6, spaces_6, 6);

    /*...o....o....o........*/
    square_t squares_7[] = {tesseract_square_read(T, 5, 4),
                            tesseract_square_read(T, 1, 0),
                            tesseract_square_read(T, 4, 4)};
    size_t spaces_7[] = {3, 4, 4};
    print_tesseract_line(squares_7, spaces_7, 3);

    /*...o..o.o.o..o........*/
    square_t squares_8[] = {tesseract_square_read(T, 5, 5),
                            tesseract_square_read(T, 1, 1),
                            tesseract_square_read(T, 1, 2),
                            tesseract_square_read(T, 1, 3),
                            tesseract_square_read(T, 4, 5)};
    size_t spaces_8[] = {3, 2, 1, 1, 2};
    print_tesseract_line(squares_8, spaces_8, 5);

    /*........o.............*/
    square_t squares_9[] = {tesseract_square_read(T, 1, 4)};
    size_t spaces_9[] = {8};
    print_tesseract_line(squares_9, spaces_9, 1);

    /*...o....o....o........*/
    square_t squares_10[] = {tesseract_square_read(T, 3, 0),
                             tesseract_square_read(T, 1, 5),
                             tesseract_square_read(T, 6, 0)};
    size_t spaces_10[] = {3, 4, 4};
    print_tesseract_line(squares_10, spaces_10, 3);

    /*.o.o.o.....o.o.o......*/
    square_t squares_11[] = {tesseract_square_read(T, 3, 1),
                             tesseract_square_read(T, 3, 2),
                             tesseract_square_read(T, 3, 3),
                             tesseract_square_read(T, 6, 1),
                             tesseract_square_read(T, 6, 2),
                             tesseract_square_read(T, 6, 3)};
    size_t spaces_11[] = {1, 1, 1, 5, 1, 1};
    print_tesseract_line(squares_11, spaces_11, 6);

    /*...o.........o........*/
    square_t squares_12[] = {tesseract_square_read(T, 3, 4),
                             tesseract_square_read(T, 6, 4)};
    size_t spaces_12[] = {3, 9};
    print_tesseract_line(squares_12, spaces_12, 2);

    /*...o.........o....o...*/
    square_t squares_13[] = {tesseract_square_read(T, 3, 5),
                             tesseract_square_read(T, 6, 5),
                             tesseract_square_read(T, 7, 0)};
    size_t spaces_13[] = {3, 9, 4};
    print_tesseract_line(squares_13, spaces_13, 3);

    /*........o.......o.o.o.*/
    square_t squares_14[] = {tesseract_square_read(T, 2, 0),
                             tesseract_square_read(T, 7, 1),
                             tesseract_square_read(T, 7, 2),
                             tesseract_square_read(T, 7, 3)};
    size_t spaces_14[] = {8, 7, 1, 1};
    print_tesseract_line(squares_14, spaces_14, 4);

    /*......o.o.o.......o...*/
    square_t squares_15[] = {tesseract_square_read(T, 2, 1),
                             tesseract_square_read(T, 2, 2),
                             tesseract_square_read(T, 2, 3),
                             tesseract_square_read(T, 7, 4)};
    size_t spaces_15[] = {6, 1, 1, 7};
    print_tesseract_line(squares_15, spaces_15, 4);
    
    /*........o.........o...*/
    square_t squares_16[] = {tesseract_square_read(T, 2, 4),
                             tesseract_square_read(T, 7, 5)};
    size_t spaces_16[] = {8, 9};
    print_tesseract_line(squares_16, spaces_16, 2);

    /*........o.............*/
    square_t squares_17[] = {tesseract_square_read(T, 2, 5)};
    size_t spaces_17[] = {8};
    print_tesseract_line(squares_17, spaces_17, 1);
}

void tesseract_free_cube(tesseract_t T, size_t cube)
/*requires C != NULL*/
/*requires cube < 8*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
{
    REQUIRES(T != NULL);
    REQUIRES(cube < 8);

    switch(cube) {
        case 0:
            cube_free(T->top);
            break;
        case 1:
            cube_free(T->center);
            break;
        case 2:
            cube_free(T->bottom);
            break;
        case 3:
            cube_free(T->front);
            break;
        case 4:
            cube_free(T->back);
            break;
        case 5:
            cube_free(T->left);
            break;
        case 6:
            cube_free(T->right);
            break;
        case 7:
            cube_free(T->rightmost);
            break;
    }
}

void tesseract_free(tesseract_t T)
/*requires T != NULL*/
{
    REQUIRES(T != NULL);
    cube_free(T->top);
    cube_free(T->center);
    cube_free(T->bottom);
    cube_free(T->front);
    cube_free(T->back);
    cube_free(T->left);
    cube_free(T->right);
    cube_free(T->rightmost);
    free(T);
}
