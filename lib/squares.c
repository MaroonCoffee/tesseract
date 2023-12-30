#include <stdlib.h>
#include <stdio.h>
#include "contracts.h"
#include "squares.h"

struct square_header {
    char *data;
    size_t len;
};

square* square_new(size_t len)
/*requires len > 0*/
/*ensures result != NULL*/
{
    REQUIRES(len > 0);
    square *S = malloc(sizeof(square));
    char *D = calloc(len*len, sizeof(char));
    S->data = D;
    S->len = len;

    square *result = S;
    ENSURES(result != NULL);
    return result;
}

size_t square_length(square* S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    return S->len;
}

size_t coord_index(size_t x, size_t y, size_t len)
{
    return (y*len) + x;
}

char square_read(square* S, size_t x, size_t y)
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/
{
    REQUIRES(S != NULL);
    REQUIRES(square_length(S) > x && square_length(S) > y);
    return (S->data)[coord_index(x, y, S->len)];
}

void square_write(square* S, char c, size_t x, size_t y)
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/
{
    REQUIRES(S != NULL);
    REQUIRES(square_length(S) > x && square_length(S) > y);
    (S->data)[coord_index(x, y, S->len)] = c;
}

void square_print(square* S)
/*requires S != NULL*/
/*empty characters displayed as `*/
{
    REQUIRES(S != NULL);
    for (size_t y = 0; y < S->len; y++){
        for (size_t x = 0; x < S->len; x++){
            char c = square_read(S, x, y);
            if (c == 0) c = '`';
            printf("%c", c);
        }
        printf("\n");
    }
}

void square_free(square* S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    free(S->data);
    free(S);
}
