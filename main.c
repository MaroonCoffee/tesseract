#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "lib/contracts.h"
#include "lib/read_script.h"
#include "lib/tesseracts.h"

#define MAX_FILENAME_LENGTH 256

void errorHandler(const char *errorMessage){
    fprintf(stderr, "Error: %s\n", errorMessage);
    exit(EXIT_FAILURE);
}

bool is_valid_input(int argc, char *argv[])
{
    //checks to see if 2 arguments were passed in
    if (argc > 2){
        errorHandler("Too many arguments passed in!");
    }
    if (argc < 2){
        errorHandler("Too few arguments passed in!");
    }
    ASSERT(argc == 2);
    
    //checks to see if filename is valid
    size_t len = strlen(argv[1]);
    if (len >= MAX_FILENAME_LENGTH){
        errorHandler("Filename is too long!");
    }
    if (argv[1][0] == ' '){
        errorHandler("Invalid filename!");
    }
    for (size_t i=0; argv[1][i] != '\0'; i++){
        char c = argv[1][i];
        if (!isalnum(c) && !(c == '_') && !(c == '-') && !(c == ',')
            && !(c == '.') && !(c == '(') && !(c == ')') && !(c == ' ')
            && !(c == '/')){
            errorHandler("Invalid filename!");
        }
    }
    if (len < 4 || strcmp(&(argv[1][len-4]), ".tes") != 0){
        errorHandler("Invalid file extension!");
    }

    //checks to see that file exists with name
    if (access(argv[1], F_OK) == -1){
        errorHandler("File does not exist!");
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (!is_valid_input(argc, argv)){
        return EXIT_FAILURE;
    }
    tesseract_t T = read_script(argv[1]);
    tesseract_print(T);
    tesseract_free(T);
    return EXIT_SUCCESS;
}
