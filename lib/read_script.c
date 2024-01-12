#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "contracts.h"
#include "tesseracts.h"
#include "xalloc.h"
#include "cubes.h"
#include "read_script.h"

typedef struct script_header script;
struct script_header {
    char **data;
    size_t size;
};

script* filename_to_script(const char *filename)
/*requires strlen(filename) > 0 && strlen(filename) <= 256*/
/*ensures result != NULL*/
{
    REQUIRES(strlen(filename) > 0 && strlen(filename) <= 256);
    script *S = xmalloc(sizeof(script));
    FILE *f = fopen(filename, "r");
    if (f == NULL){
        printf("Error: The file %s cannot be oppened", filename);
        abort();
    }
    
    //buffer to store the program length in: init_buffer[2]
    char init_buffer[4];
    if (fgets(init_buffer, 4, f) == NULL){
        ASSERT(false);
    }
    
    if (!isdigit(init_buffer[2])){
        ASSERT(false);
    }
    size_t script_size = (size_t) (init_buffer[2] - '0');
    S->size = script_size;

    //allocates 17*script_size rows of cells and 18 rows of dots
    char **data = xmalloc((17 * script_size + 18) * sizeof(char*));
    S->data = data;
    //buffer that script lines will be read into
    char buffer[22*script_size+5];
    
    //stores the first line of the program
    buffer[0] = init_buffer[2];
    if (fgets(&buffer[1], 22*script_size+3, f) == NULL){
        ASSERT(false);
    }
    buffer[22*script_size] = '\0';
    data[0] = xmalloc(22*script_size*sizeof(char*));
    strcpy(data[0], buffer);
    
    //stores the remaining lines of the program
    for (size_t i=1; i<17*script_size+18; i++){
        if (fgets(buffer, 22*script_size+6, f) == NULL){
            ASSERT(false);
        }
        buffer[22*script_size + 2] = '\0';
        data[i] = xmalloc(22*script_size*sizeof(char*));
        strcpy(data[i], &buffer[2]);
    }

    ASSERT(feof(f));
    ASSERT(strlen(S->data[17 * script_size + 17])==22*script_size);
    fclose(f);
    
    script *result = S;
    ENSURES(result != NULL);
    return result;
}

void free_script(script *S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    for (size_t i=0; i<17*S->size+18; i++){
        free(S->data[i]);
    }
    free(S->data);
    free(S);
}

square_t script_to_square(script *S, size_t x, size_t y)
/*requires S != NULL*/
/*requires x < 22*S->size*/
/*requires y < 17*S->size+18*/
/*ensures result != NULL*/
/*(x, y) are the top left coords of the square*/
{
    REQUIRES(S != NULL);
    REQUIRES(x < 22*S->size);
    REQUIRES(y < 17*S->size + 18);

    size_t len = S->size;
    square_t square = square_new(len);
    for (size_t sqry=0; sqry<len; sqry++){
        for (size_t sqrx=0; sqrx<len; sqrx++){
            square_write(square, S->data[y+sqry][x+sqrx], sqrx, sqry);
        }
    }

    square_t result = square;
    ENSURES(result != NULL);
    return result;
}

cube_t script_to_cube(script *S, size_t x, size_t y)
/*requires S != NULL*/
/*requires x < 22*S->size*/
/*requires y < 17*S->size+18*/
/*ensures result != NULL*/
/*(x, y) are the top left coords of the center face of the cube*/
/*0=left, 1=bot, 2=front, 3=top, 4=right, 5=back*/
{
    REQUIRES(S != NULL);
    REQUIRES(x < 22*S->size);
    REQUIRES(y < 17*S->size + 18);

    size_t len = S->size;
    cube_t C = cube_new(len);

    square_t left_face = script_to_square(S, x, y-(1+len));
    square_t bottom_face = script_to_square(S, x-(2*len), y);
    square_t center_face = script_to_square(S, x, y);
    square_t top_face = script_to_square(S, x+(2*len), y);
    square_t right_face = script_to_square(S, x, y+(1+len));
    square_t back_face = script_to_square(S, x, y+2*(1+len));

    cube_write(C, left_face, 0);
    cube_write(C, bottom_face, 1);
    cube_write(C, center_face, 2);
    cube_write(C, top_face, 3);
    cube_write(C, right_face, 4);
    cube_write(C, back_face, 5);

    cube_t result = C;
    ENSURES(result != NULL);
    return result;
}

tesseract_t script_to_tesseract(script *S)
/*requires S != NULL*/
/*ensures result != NULL*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
{
    REQUIRES(S != NULL);

    size_t len = S->size;
    tesseract_t T = tesseract_new(len);

    cube_t top = script_to_cube(S, 8*len, 2+len);
    cube_t center = script_to_cube(S, 8*len, 8+7*len);
    cube_t bottom = script_to_cube(S, 8*len, 15+14*len);
    cube_t front = script_to_cube(S, 3*len, 11+10*len);
    cube_t back = script_to_cube(S, 13*len, 6+5*len);
    cube_t left = script_to_cube(S, 3*len, 6+5*len);
    cube_t right = script_to_cube(S, 13*len, 11+10*len);
    cube_t right_most = script_to_cube(S, 18*len, 14+13*len);

    tesseract_write(T, top, 0);
    tesseract_write(T, center, 1);
    tesseract_write(T, bottom, 2);
    tesseract_write(T, front, 3);
    tesseract_write(T, back, 4);
    tesseract_write(T, left, 5);
    tesseract_write(T, right, 6);
    tesseract_write(T, right_most, 7);

    tesseract_t result = T;
    ENSURES(result != NULL);
    return result;
}

script* filename_to_cube_script(const char *filename)
/*requires strlen(filename) > 0 && strlen(filename) <= 256*/
/*ensures result != NULL*/
{
    REQUIRES(strlen(filename) > 0 && strlen(filename) <= 256);
    script *S = xmalloc(sizeof(script));
    FILE *f = fopen(filename, "r");
    if (f == NULL){
        printf("Error: The file %s cannot be oppened", filename);
        abort();
    }
    
    //buffer to store the program length in: init_buffer[0]
    char init_buffer[2];
    if (fgets(init_buffer, 2, f) == NULL){
        ASSERT(false);
    }
    if (!isdigit(init_buffer[0])){
        ASSERT(false);
    }
    size_t script_size = (size_t) (init_buffer[0] - '0');
    S->size = script_size;

    //allocates 4*script_size rows of cells and 5 rows of dots
    char **data = xmalloc((4 * script_size + 5) * sizeof(char*));
    S->data = data;
    //buffer that script lines will be read into
    char buffer[3*script_size+5];
    
    //stores the first line of the program
    buffer[0] = init_buffer[0];
    if (fgets(&buffer[1], 3*script_size+5, f) == NULL){
        ASSERT(false);
    }
    buffer[3*script_size+4] = '\0';
    data[0] = xmalloc((3*script_size+4)*sizeof(char*));
    strcpy(data[0], buffer);
    
    //stores the remaining lines of the program
    for (size_t i=1; i<4*script_size+5; i++){
        if (fgets(buffer, 3*script_size+6, f) == NULL){
            ASSERT(false);
        }
        buffer[3*script_size+4] = '\0';
        data[i] = xmalloc((3*script_size+4)*sizeof(char*));
        strcpy(data[i], buffer);
    }

    ASSERT(feof(f));
    fclose(f);
    
    script *result = S;
    ENSURES(result != NULL);
    return result;
}

void free_cube_script(script *S)
/*requires S != NULL*/
{
    REQUIRES(S != NULL);
    for (size_t i=0; i<4*S->size+5; i++){
        free(S->data[i]);
    }
    free(S->data);
    free(S);
}

square_t cube_script_to_square(script *S, size_t x, size_t y)
/*requires S != NULL*/
/*requires x < 3*S->size+4*/
/*requires y < 4*S->size+5*/
/*ensures result != NULL*/
/*(x, y) are the top left coords of the square*/
{
    REQUIRES(S != NULL);
    REQUIRES(x < 3*S->size+4);
    REQUIRES(y < 4*S->size+5);

    size_t len = S->size;
    square_t square = square_new(len);
    for (size_t sqry=0; sqry<len; sqry++){
        for (size_t sqrx=0; sqrx<len; sqrx++){
            square_write(square, S->data[y+sqry][x+sqrx], sqrx, sqry);
        }
    }

    square_t result = square;
    ENSURES(result != NULL);
    return result;
}

tesseract_t cube_script_to_tesseract(script *S)
/*requires S != NULL*/
/*ensures result != NULL*/
/*0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 6=right, 7=rightmost*/
{
    REQUIRES(S != NULL);

    size_t len = S->size;
    tesseract_t T = tesseract_initialize(tesseract_new(len));

    tesseract_free_cube(T, 0);

    cube_t top = cube_new(len);

    square_t left_face = cube_script_to_square(S, 2+len, 1);
    square_t bottom_face = cube_script_to_square(S, 1, 2+len);
    square_t center_face = cube_script_to_square(S, 2+len, 2+len);
    square_t top_face = cube_script_to_square(S, 3+2*len, 2+len);
    square_t right_face = cube_script_to_square(S, 2+len, 3+2*len);
    square_t back_face = cube_script_to_square(S, 2+len, 4+3*len);

    cube_write(top, left_face, 0);
    cube_write(top, bottom_face, 1);
    cube_write(top, center_face, 2);
    cube_write(top, top_face, 3);
    cube_write(top, right_face, 4);
    cube_write(top, back_face, 5);

    tesseract_write(T, top, 0);

    tesseract_t result = T;
    ENSURES(result != NULL);
    return result;
}

tesseract_t read_script(const char *filename, bool is_tesseract)
/*requires strlen(filename) > 0 && strlen(filename) <= 256*/
/*ensures result != NULL*/
{
    REQUIRES(strlen(filename) > 0 && strlen(filename) <= 256);
    tesseract_t T;
    if (is_tesseract){
        script *S = filename_to_script(filename);
        T = script_to_tesseract(S);
        free_script(S);
    }
    else {
        script *S = filename_to_cube_script(filename);
        T = cube_script_to_tesseract(S);
        free_cube_script(S);
    }
    tesseract_t result = T;
    ENSURES(result != NULL);
    return result;
}
