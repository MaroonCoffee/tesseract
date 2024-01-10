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
    if (len < 4 || (strcmp(&(argv[1][len-4]), ".tes") != 0
        && strcmp(&(argv[1][len-5]), ".cube") != 0
        && strcmp(&(argv[1][len-5]), ".dewy") != 0)) {
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

//Three file extensions: .tes .cube .dewy (switching modes based on type)
//Cube files read in with special function and in center cube
//Implement the following instructions
// ( : Begins execution at this point
// ) : Ends execution at this point
// > : Turn rightchatgpt
// < : Turn left
// ^ : Turn up
// v : Turn down
//direction gets changed when switching cube faces
//LetNum Stack: Stores letters and numbers (no v, and space is ~)
//Operation Stack: + - * / %
//Characters placed in correct stack automatically
// " : Key character that swaps do operation / don't (starts on don't)
// | : Prints "Hello World"
// ; : Pops num off "n" then print next n chars
// , : Takes input and adds characters in order on to stack
// ` : Clears both stacks
// = : Pops top off and prints ascii character of it 
// ! : Switch stack that stack_modifiers modify (starts on LetNum Stack)
// # : Duplicate top character on stack (chosen by !)
// $ : Pops num off "n" then duplicates the nth character on ! stack to top
// & : Pops num off "n" then deletes the nth character on ! stack
// @ : Pops nums off "m" and "n" then swaps values on ! stack
// ? : Pops nums off "m" and "n". Turn right if m > n, left if m < n, straight =
// : : Pops nums off "n" then until colon hit n times turn left, after straight
//     This is a bit more involved: Will require dictionary and for loop type
