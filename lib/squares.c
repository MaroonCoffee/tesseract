#include <stdlib.h>
#include <stdio.h>
#include "contracts.h"
#include "squares.h"
#include "xalloc.h"

#define FILLER_CHAR '`'
#define EMPTY_CHAR ' '

struct square_header {
    char *data;
    size_t len;
};

square_t square_new(size_t len)
/*requires len > 0*/
/*ensures result != NULL*/
{
    REQUIRES(len > 0);
    square *S = xmalloc(sizeof(square));
    char *D = xcalloc(len*len, sizeof(char));
    S->data = D;
    S->len = len;

    square *result = S;
    ENSURES(result != NULL);
    return result;
}

size_t square_length(square_t S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    return S->len;
}

size_t coord_index(size_t x, size_t y, size_t len)
{
    return (y*len) + x;
}

char square_read(square_t S, size_t x, size_t y)
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/
{
    REQUIRES(S != NULL);
    REQUIRES(square_length(S) > x && square_length(S) > y);
    return (S->data)[coord_index(x, y, S->len)];
}

void square_write(square_t S, char c, size_t x, size_t y)
/*requires S != NULL*/
/*requires square_length(S) > x && square_length(S) > y*/
{
    REQUIRES(S != NULL);
    REQUIRES(square_length(S) > x && square_length(S) > y);
    (S->data)[coord_index(x, y, S->len)] = c;
}

void square_print(square_t S)
/*requires S != NULL*/
/*empty characters displayed as o*/
/*border characters displayed as `*/
{
    REQUIRES(S != NULL);
    printf("%c", FILLER_CHAR);
    for (size_t i = 0; i< S->len; i++){
        printf("%c", FILLER_CHAR);
    }
    printf("%c\n", FILLER_CHAR);
    
    for (size_t y = 0; y < S->len; y++){
        printf("%c", FILLER_CHAR);
        for (size_t x = 0; x < S->len; x++){
            char c = square_read(S, x, y);
            if (c == 0) c = EMPTY_CHAR;
            printf("%c", c);
        }
        printf("%c\n", FILLER_CHAR);
    }

    printf("%c", FILLER_CHAR);
    for (size_t i = 0; i< S->len; i++){
        printf("%c", FILLER_CHAR);
    }
    printf("%c\n", FILLER_CHAR);
}

void square_free(square_t S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    free(S->data);
    free(S);
}
