#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "lib/stacks.h"
#include "lib/contracts.h"
#include "lib/read_script.h"
#include "lib/loops.h"
#include "lib/xalloc.h"
#include "lib/tesseracts.h"

#define MAX_FILENAME_LENGTH 256
#define EMPTY_CHAR ' '
#define DASH_CHAR '-'

typedef struct alpha_num_header alphanum; 
struct alpha_num_header {
    bool is_char;
    int i;
};

void free_alphanum(void *e)
/*requires e != NULL*/
{
    REQUIRES(e != NULL);
    alphanum *a = (alphanum*) e;
    free(a);
}

void free_operation(void *e)
/*requires e != NULL*/
{
    REQUIRES(e != NULL);
    char *c = (char*) e;
    free(c);
}

void print_n_dashes(size_t n)
{
    for (size_t i=0; i<n; i++){
        printf("%c", DASH_CHAR);
    }
    printf("\n");
}

void print_header(size_t n, const char *title)
{
    print_n_dashes(n);
    printf("%s\n", title);
    print_n_dashes(n);
}

void print_alphanum(alphanum *a)
{
    if (a->is_char){
        printf("%c", a->i);
    }
    else {
        printf("%d", a->i);
    }
}

void print_letnum_elem(genstack_elem e)
{
    alphanum *a = (alphanum*) e;
    if (a->is_char){
        printf("%c\n", a->i);
    }
    else {
        printf("%d\n", a->i);
    }
}

void print_op_elem(genstack_elem e)
{
    char c = *((char*) e);
    printf("%c\n", c);
}

void print_debug(tesseract_t T, genstack_t ln_stack, genstack_t op_stack,
                 bool pt, bool op_on_wrap, bool ln_stack_m, bool print_cursor,
                 bool cube_m, bool str_m, bool bdg_m)
/*requires T != NULL*/
{
    REQUIRES(T != NULL);
    size_t dash_len = tesseract_length(T) * 22 + 4;
    
    print_header(dash_len, "Tesseract");
    tesseract_print(T, print_cursor);

    print_header(dash_len, "LetNum Stack");
    stack_print(ln_stack, &print_letnum_elem);

    print_header(dash_len, "Operation Stack");
    stack_print(op_stack, &print_op_elem);

    print_header(dash_len, "Loop (':') Dictionary");
    loop_dict_print(tesseract_get_loop_dict(T), tesseract_length(T));
    print_n_dashes(dash_len);

    printf("program_terminated: %s\n", pt ? "true" : "false");
    printf("operation_on_wrap: %s\n", op_on_wrap ? "true" : "false");
    printf("letnum_stack_mode: %s\n", ln_stack_m ? "true" : "false");
    printf("cube_mode: %s\n", cube_m ? "true" : "false");
    printf("string_mode: %s\n", str_m ? "true" : "false");
    printf("bridge_mode: %s\n", bdg_m ? "true" : "false");
    print_n_dashes(dash_len);
}

int pop_letnum_int(genstack_t letnum_stack, size_t *error, bool cubemode)
{
    if (!stack_empty(letnum_stack)){
        alphanum *a = (alphanum*) stack_pop(letnum_stack);
        bool is_char = a->is_char;
        int n = a->i;
        free_alphanum((void*) a);
        if (!is_char){
            return n;
        }
        else {
            /*Expected int but found char on LetNum stack!*/
            *error = 7;
        }
    }
    else if (!cubemode){
        return 0;
    }
    else {
        /*Attempted to pop off empty LetNum stack!*/
        *error = 6;
    }
    return -1;
}

size_t pop_2_letnum_ints(genstack_t letnum_stack, int *i, int *j, bool cubemode)
{
    if (stack_size(letnum_stack) >= 2){
        alphanum *m = stack_pop(letnum_stack);
        alphanum *n = stack_pop(letnum_stack);
        bool are_ints = !m->is_char && !n->is_char;
        *i = m->i;
        *j = n->i;
        free_alphanum((void*) m);
        free_alphanum((void*) n);
        if (are_ints){
            return 0;
        }
        else {
            /*Expected int but found char on LetNum stack!*/
            return 7;
        }
    }
    else if (!cubemode){
        *j = 0;
        if (stack_empty(letnum_stack)){
            *i = 0;
            return 0;
        }
        else {
            size_t error = 0;
            int temp = pop_letnum_int(letnum_stack, &error, cubemode);
            if (error == 0){
                *i = temp;
                return 0;
            }
            else{
                return error;
            }
        }
    }
    else {
        /*Attempted to pop off empty LetNum stack!*/
        return 6;
    }
}

void errorHandler(size_t error)
{
    char *errorMessage;
    switch(error){
        case 1:
            errorMessage = "Wrong numer of arguments passed in (must be 1)";
            break;
        case 2:
            errorMessage = "Filename is too long!";
            break;
        case 3:
            errorMessage = "Invalid filename!";
            break;
        case 4:
            errorMessage = "Invalid file extension!";
            break;
        case 5:
            errorMessage = "File does not exist!";
            break;
        case 6:
            errorMessage = "Attempted to pop off empty LetNum stack!";
            break;
        case 7:
            errorMessage = "Expected int but found char on LetNum stack!";
            break;
        case 8:
            errorMessage = "Error reading input!";
            break;
        case 9:
            errorMessage = "Attempted to pop off empty operation stack!";
            break;
        case 10:
            errorMessage = "Stack index out of bounds!";
            break;
        case 11:
            errorMessage = "Attempted to parse unknown operation! (Bug)";
            break;
        default:
            errorMessage = "An unknown error has occured! (Bug)";
            break;
    }
    fprintf(stderr, "Error: %s\n", errorMessage);
    exit(EXIT_FAILURE);
}

bool is_file_extension(const char *ext, char *argv[])
{
    return strcmp(&(argv[1][strlen(argv[1])-strlen(ext)]), ext) == 0;
}

bool is_valid_input(int argc, char *argv[])
{
    /*checks to see if 2 arguments were passed in*/
    if (argc > 2){
        /*Too many arguments passed in!*/
        errorHandler(1);
    }
    if (argc < 2){
        /*Too many arguments passed in!*/
        errorHandler(1);
    }
    ASSERT(argc == 2);
    
    /*checks to see if filename is valid*/
    size_t len = strlen(argv[1]);
    if (len >= MAX_FILENAME_LENGTH){
        /*Filename is too long!*/
        errorHandler(2);
    }
    if (argv[1][0] == ' '){
        /*Invalid filename!*/
        errorHandler(3);
    }
    for (size_t i=0; argv[1][i] != '\0'; i++){
        char c = argv[1][i];
        if (!isalnum(c) && !(c == '_') && !(c == '-') && !(c == ',')
            && !(c == '.') && !(c == '(') && !(c == ')') && !(c == ' ')
            && !(c == '/')){
            /*Invalid filename!*/
            errorHandler(3);
        }
    }
    if (len < 4 || (!is_file_extension(".tes", argv) && 
                    !is_file_extension(".cube", argv) && 
                    !is_file_extension(".dewy", argv))) {
        /*Invalid file extension!*/
        errorHandler(4);
    }

    /*checks to see that file exists with name*/
    if (access(argv[1], F_OK) == -1){
        /*File does not exist!*/
        errorHandler(5);
    }

    return true;
}

size_t execute_operation(genstack_t operation_stack, genstack_t letnum_stack)
{
    if (!stack_empty(operation_stack)){
        char *op_pointer = (char*) stack_pop(operation_stack);
        char operation = *op_pointer;
        free_operation((void*) op_pointer);
        int i = 0;
        int j = 0;
        size_t error = pop_2_letnum_ints(letnum_stack, &i, &j, true);
        if (error == 0){
            switch (operation) {
                case '+': {
                    int res = i+j;
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = res;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                    return 0;
                }

                case '-': {
                    int res = i-j;
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = res;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                    return 0;
                }

                case '*': {
                    int res = i*j;
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = res;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                    return 0;
                }

                case '/': {
                    int res = i/j;
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = res;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                    return 0;
                }
                
                case '%': {
                    int res = i%j;
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = res;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                    return 0;
                }

                default: {
                    /*Attempted to parse unknown operation!*/
                    return 11;
                }
            }
        }
        else {
            return error;
        }
    }
    else {
        /*Attempted to pop off empty operation stack!*/
        return 9;
    }
}

int main(int argc, char *argv[])
{
    if (!is_valid_input(argc, argv)){
        return EXIT_FAILURE;
    }
    tesseract_t T;
    if (is_file_extension(".tes", argv)){
        T = read_script(argv[1], true);
    }
    else {
        T = read_script(argv[1], false);
    }
    cursor_t cursor = tesseract_cursor(T);
    tesseract_initial_parse(T);
    
    size_t exit_code = 0;
    bool program_terminated = false;
    bool operation_on_wrap = false;
    bool letnum_stack_mode = true;
    bool cube_mode = true;
    bool string_mode = false;
    bool bridge_mode = false;
    genstack_t letnum_stack = stack_new();
    genstack_t operation_stack = stack_new();

    while (!program_terminated){
        bool side_switch = cursor_move_next(tesseract_cursor(T));
        char symbol = tesseract_cursor_read(T);
        bool alpha_num_symbol = false;

        /*Execution of operations on cube wrap if mode toggled on*/
        if (side_switch){
            side_switch = false;
            if (operation_on_wrap){
                exit_code = execute_operation(operation_stack, letnum_stack);
                if (exit_code != 0){
                    program_terminated = true;
                    break;
                }
            }
        }
        
        /*All characters are pushed to the LetNum stack as their ascii value
        if mode toggled on*/
        if (string_mode && !cube_mode){
            if (symbol == '"'){
                string_mode = false;
            }
            else {
                alphanum *a = xmalloc(sizeof(alphanum));
                a->i = symbol;
                a->is_char = false;
                stack_push(letnum_stack, (void*) a);
            }
            symbol = EMPTY_CHAR;
        }

        /*If the previous character was a bridge character and script is in
        Befunge mode, the current command is skipped*/
        if (bridge_mode && !cube_mode){
            bridge_mode = false;
            symbol = EMPTY_CHAR;
        }

        IF_DEBUG(print_debug(T, letnum_stack, operation_stack,
                             program_terminated, operation_on_wrap, 
                             letnum_stack_mode, true, cube_mode, string_mode,
                             bridge_mode));

        switch(symbol) {
            /*When the pointer reaches a blank space, nothing happens*/
            case EMPTY_CHAR: {
                break;
            }
            
            /*Both Modes: Spawns the cursor. If multiple '(' are present in the 
            script, the first '(' is where the cursor is spawned. Order is 
            determined by lowest cube index, then lowest square index, then 
            lowest cell index. 
            Cube indices: 0=top, 1=center, 2=bot, 3=front, 4=back, 5=left, 
            6=right, 7=rightmost. 
            Square indices: 0=left, 1=bot, 2=front, 3=top, 4=right, 5=back
            Nothing happens when executed*/
            case '(': {
                break;
            }

            /*Cube Mode: Terminates the program
            Befunge Mode: Nothing happens*/    
            case ')': {
                if (cube_mode){
                    program_terminated = true;
                }
                break;
            }

            /*Both Modes: Changes the cursor direction to up*/
            case '^': {
                cursor_dir_set(cursor, 0);
                break;
            }

            /*Both Modes: Changes the cursor direction to right*/
            case '>': {
                cursor_dir_set(cursor, 1);
                break;
            }

            /*Both Modes: Changes the cursor direction to bottom*/
            case 'v': {
                cursor_dir_set(cursor, 2);
                break;
            }

            /*Both Modes: Changes the cursor direction to left*/
            case '<': {
                cursor_dir_set(cursor, 3);
                break;
            }

            /*Cube Mode: Adds the addition operator onto the operation stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n), then pushes
            m+n to the LetNum stack*/
            case '+': {
                char *operation = xmalloc(sizeof(char));
                *operation = '+';
                stack_push(operation_stack, (void*) operation);
                if (!cube_mode){
                    exit_code = execute_operation(operation_stack,letnum_stack);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Adds the subtraction operator onto the operation stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n), then pushes
            m-n to the LetNum stack*/
            case '-': {
                char *operation = xmalloc(sizeof(char));
                *operation = '-';
                stack_push(operation_stack, (void*) operation);
                if (!cube_mode){
                    exit_code = execute_operation(operation_stack,letnum_stack);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Adds the mult. operator onto the operation stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n), then pushes
            m*n to the LetNum stack*/
            case '*': {
                char *operation = xmalloc(sizeof(char));
                *operation = '*';
                stack_push(operation_stack, (void*) operation);
                if (!cube_mode){
                    exit_code = execute_operation(operation_stack,letnum_stack);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Adds the divison operator onto the operation stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n), then pushes
            m/n to the LetNum stack*/
            case '/': {
                char *operation = xmalloc(sizeof(char));
                *operation = '/';
                stack_push(operation_stack, (void*) operation);
                if (!cube_mode){
                    exit_code = execute_operation(operation_stack,letnum_stack);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Adds the modulo operator onto the operation stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n), then pushes
            m%n to the LetNum stack*/
            case '%': {
                char *operation = xmalloc(sizeof(char));
                *operation = '%';
                stack_push(operation_stack, (void*) operation);
                if (!cube_mode){
                    exit_code = execute_operation(operation_stack,letnum_stack);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Adds the space character to the LetNum stack
            Befunge Mode: Gets an char from the user and pushes ascii value onto 
            the LetNum stack*/
            case '~': {
                if (cube_mode){
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = ' ';
                    a->is_char = true;
                    stack_push(letnum_stack, (void*) a);
                }
                else {
                    printf("Singular int required: ");
                    char c = getchar();
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = (int) c;
                    a->is_char = false;
                    stack_push(letnum_stack, (void*) a);
                }
                break;
            }

            /*Cube Mode: Toggles the execution of operations on cube wrap 
            (starts on)
            Befunge Mode: Starts string mode, where all characters are pushed
            to the LetNum stack as their ascii value until the next '"' is read
            (string mode has no affect in cube mode)*/
            case '"': {
                if (cube_mode){
                    operation_on_wrap = !operation_on_wrap;
                }
                else {
                    string_mode = true;
                }
                break;
            }

            /*Cube Mode: Prints "Hello World"
            Befunge Mode: Pops an int off the LetNum stack (n). Moves the cursor
            down if n=0, otherwise moves the cursor up*/
            case '|': {
                if (cube_mode){
                    printf("Hello World\n");
                }
                else {
                    int n = pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code == 0){
                        if (n == 0){
                            cursor_dir_set(cursor, 2);
                        }
                        else {
                            cursor_dir_set(cursor, 0);
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Pops an int off the LetNum stack (n) then pops and 
            prints n characters from the LetNum stack
            Befunge Mode: Not a befunge command and is therefore ignored*/
            case ';': {
                if (cube_mode){
                    int n = pop_letnum_int(letnum_stack, &exit_code, true);
                    if (exit_code == 0){
                        if (n <= (int) stack_size(letnum_stack)){
                                for (int i=0; i<n; i++){
                                    void *e = stack_pop(letnum_stack);
                                    print_alphanum((alphanum*) e);
                                    free_alphanum(e);
                                }
                                printf("\n");
                            }
                            else {
                                /*Attempted to pop off empty LetNum stack!*/
                                exit_code = 6;
                                program_terminated = true;
                            }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Gets input from the user and pushes input onto the 
            LetNum stack
            Befunge Mode: Pops an int off the LetNum stack (n) and prints it as
            an ascii character*/
            case ',': {
                if (cube_mode){
                    char input[256];
                    printf("Input required: ");
                    if (fgets(input, 256, stdin) != NULL){
                        size_t len = strlen(input);
                        if (len-1 > 0 && input[len - 1] == '\n') {
                            input[len - 1] = '\0';
                        }
                        for (size_t i = 0; i < len-1; i++) {
                            alphanum *a = xmalloc(sizeof(alphanum));
                            char c = input[i];
                            if (isdigit(c)){
                                a->i = c - '0';
                                a->is_char = false;
                            }
                            else{
                                a->i = c;
                                a->is_char = true;
                            }
                            stack_push(letnum_stack, (void*) a);
                        }
                    }
                    else {
                        /*Error reading input!*/
                        exit_code = 8;
                        program_terminated = true;
                    }
                }
                else {
                    int n = pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code == 0){
                        printf("%c", n);
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Clears both the LetNum and Operation Stack
            Befunge Mode: Pops 2 ints off the LetNum stack (m, n). If n > a
            pushes 1 onto the LetNum stack, otherwise pushes 0*/
            case '`': {
                if (cube_mode){
                    stack_free(letnum_stack, &free_alphanum);
                    stack_free(operation_stack, &free_operation);
                    letnum_stack = stack_new();
                    operation_stack = stack_new();
                }
                else{
                    int i = 0;
                    int j = 0;
                    exit_code = pop_2_letnum_ints(letnum_stack, &i, &j, false);
                    if (exit_code == 0){
                        alphanum *a = xmalloc(sizeof(alphanum));
                        a->is_char = false;
                        if (j > i){
                            a->i = 1;
                        }
                        else{
                            a->i = 0;
                        }
                        stack_push(letnum_stack, (void*) a);
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Pops an int off the LetNum stack (n), then pushes n's
            ascii representation as a char to the LetNum stack
            Befunge Mode: Not a befunge command and is therefore ignored*/
            case '=': {
                if (cube_mode){
                    int i = pop_letnum_int(letnum_stack, &exit_code, true);
                    if (exit_code == 0){
                        alphanum *a = xmalloc(sizeof(alphanum));
                        a->i = i;
                        a->is_char = true;
                        stack_push(letnum_stack, (void*) a);
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Toggles the stack mode so stack operations occur on 
            the other stack (starts on LetNum mode)
            Befunge Mode: Pops an int off the LetNum stack (n). If n is 0 pushes
            1 onto the LetNum stack, otherwise pushes 0*/
            case '!': {
                if (cube_mode){
                    letnum_stack_mode = !letnum_stack_mode;
                }
                else {
                    int i = pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code == 0){
                        alphanum *a = xmalloc(sizeof(alphanum));
                        a->is_char = false;
                        if (i == 0){
                            a->i = 1;
                        }
                        a->i = 0;
                        stack_push(letnum_stack, (void*) a);
                    }
                    else{
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Duplicates the top character on the chosen stack 
            (determined by stack mode)
            Befunge Mode: Skips the next cell's execution*/
            case '#': {
                if (cube_mode){
                    if (letnum_stack_mode){
                        if (!stack_empty(letnum_stack)){
                            alphanum *a;
                            a = (alphanum*) stack_peek_n(letnum_stack, 0);
                            alphanum *a1 = xmalloc(sizeof(alphanum));
                            a1->i = a->i;
                            a1->is_char = a->is_char;
                            stack_push(letnum_stack, (void*) a1);
                        }
                        else {
                            /*Attempted to pop off empty LetNum stack!*/
                            exit_code = 6;
                            program_terminated = true;
                        }
                    }
                    else {
                        if (!stack_empty(operation_stack)){
                            char c;
                            c = *((char*) stack_peek_n(operation_stack, 0));
                            char *op = xmalloc(sizeof(char));
                            *op = c;
                            stack_push(operation_stack, (void*) op);
                        }
                        else {
                            /*Attempted to pop off empty operation stack!*/
                            exit_code = 9;
                            program_terminated = true;
                        }
                    }
                }
                else {
                    bridge_mode = true;
                }
                break;
            }
            
            /*Cube Mode: Pops an int off the LetNum stack (n), then duplicates 
            the nth character on the chosen stack (determined by stack mode) on 
            to the top of the stack
            Befunge Mode: Pops an int off the LetNum stack and discards it*/
            case '$': {
                if (cube_mode){
                    int n = pop_letnum_int(letnum_stack, &exit_code, true);
                    if (exit_code == 0){
                        if (letnum_stack_mode){
                            if (n-1 < (int) stack_size(letnum_stack) && n-1>=0){
                                alphanum *a;
                                a = (alphanum*) stack_peek_n(letnum_stack, n-1);
                                alphanum *a1 = xmalloc(sizeof(alphanum));
                                a1->i = a->i;
                                a1->is_char = a->is_char;
                                stack_push(letnum_stack, (void*) a1);
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                        else {
                            if (n-1 < (int) stack_size(operation_stack) && 
                                n-1>=0)
                            {
                                char c = *((char*) stack_peek_n(operation_stack,
                                                                n-1));
                                char *op = xmalloc(sizeof(char));
                                *op = c;
                                stack_push(operation_stack, (void*) op);
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                else {
                    pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code != 0){
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Pops an int off the LetNum stack (n), then deletes the 
            nth character on the chosen stack (determined by stack mode)
            Befunge Mode: Gets an int from the user and pushes input onto the 
            LetNum stack*/
            case '&': {
                if (cube_mode){
                    int n = pop_letnum_int(letnum_stack, &exit_code, true);
                    if (exit_code == 0){
                        if (letnum_stack_mode){
                            if (n-1 < (int) stack_size(letnum_stack) && n-1>=0){
                                free_alphanum(stack_pop_n(letnum_stack, n-1));
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                        else {
                            if (n-1 < (int) stack_size(operation_stack) && 
                                n-1>=0)
                            {
                                free_operation(stack_pop_n(operation_stack, 
                                                           n-1));
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                else {
                    int val = 0;
                    char input[256];
                    printf("Singular int required: ");
                    if (fgets(input, 256, stdin) != NULL){
                        size_t len = strlen(input);
                        if (len-1 > 0 && input[len - 1] == '\n') {
                            input[len - 1] = '\0';
                        }
                        for (size_t i = 0; i < len-1; i++) {
                            char c = input[i];
                            if (isdigit(c)){
                                int digit = c - '0';
                                val = val * 10;
                                val = val + digit;
                            }
                            else {
                                /*Error reading input!*/
                                exit_code = 8;
                                program_terminated = true;
                                break;
                            }
                        }
                        alphanum *a = xmalloc(sizeof(alphanum));
                        a->i = val;
                        a->is_char = false;
                        stack_push(letnum_stack, (void*) a);
                    }
                    else {
                        /*Error reading input!*/
                        exit_code = 8;
                        program_terminated = true;
                    }
                }
                break;
            }
            
            /*Cube Mode: Pops 2 ints off the LetNum stack (m, n), then swaps the 
            mth and nth characters on the curently selected stack
            Befunge Mode: Terminates the program*/
            case '@': {
                if (cube_mode){
                    int i = 0;
                    int j = 0;
                    exit_code = pop_2_letnum_ints(letnum_stack, &i, &j, true);
                    if (exit_code == 0){
                        if (letnum_stack_mode){
                            if (i-1<(int) stack_size(letnum_stack) && i-1>= 0 &&
                                j-1<(int) stack_size(letnum_stack) && j-1>= 0)
                            {
                                stack_swap_mn(letnum_stack, i-1, j-1);
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                        else {
                            if (i-1<(int) stack_size(operation_stack) && i-1>=0 
                                && j-1<(int) stack_size(operation_stack) 
                                && j-1>=0)
                            {
                                stack_swap_mn(operation_stack, i-1, j-1);
                            }
                            else {
                                /*Stack index out of bounds!*/
                                exit_code = 10;
                                program_terminated = true;
                            }
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                else {
                    program_terminated = true;
                }
                break;
            }
            
            /*Cube Mode: Pops 2 ints off the LetNum stack (m, n), then turns the
            cursor direction right if m>n, left if m<n, or unchanged if m=n.
            Intended to be used as an if statement
            Befunge Mode: Changes the cursor direction to a random direction*/
            case '?': {
                if (cube_mode){
                    int i = 0;
                    int j = 0;
                    exit_code = pop_2_letnum_ints(letnum_stack, &i, &j, true);
                    if (exit_code == 0){
                        size_t dir = cursor_get_dir(cursor);
                        if (i > j){
                            cursor_dir_set(cursor, (dir-1)%4);
                        }
                        else if (i < j){
                            cursor_dir_set(cursor, (dir+1)%4);
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                else {
                    cursor_dir_set(cursor, rand() % 4);
                }
                break;
            }
            
            /*Cube Mode: If ':' is not active: Pops an int off the LetNum stack 
            (n), then set the cell's state to n and turns the cursor left.
            If ':' is active: Decreases the cell's state. If the 
            cell's new state is greater than zero, turn the cursor left. 
            Otherwise if the cell's state is now zero, leave the cursor's 
            direction unchanged and deactivate the cell.
            Intended to be used as a for loop
            Befunge Mode: Duplicates the top character on the LetNum stack*/

            case ':': {
                if (cube_mode){
                    size_t dir = cursor_get_dir(cursor);
                    int key = loop_key(cursor_get_cube(cursor), 
                                        cursor_get_square(cursor), 
                                        cursor_get_x(cursor), 
                                        cursor_get_y(cursor), 
                                        tesseract_length(T));
                    loop_t loop = loop_dict_lookup(tesseract_get_loop_dict(T), 
                                                   key);
                    if (loop == NULL){
                        loop_init(tesseract_get_loop_dict(T), key);
                        loop = loop_dict_lookup(tesseract_get_loop_dict(T), 
                                                key);
                    }
                    ASSERT(loop != NULL);
                    if (is_loop_active(loop)){
                        loop_decrease(loop);
                        if (is_loop_active(loop)){
                            cursor_dir_set(cursor, (dir+1)%4);
                        }
                    }
                    else{
                        int n = pop_letnum_int(letnum_stack, &exit_code, true);
                        if (exit_code == 0){
                            if (n > 0){
                                cursor_dir_set(cursor, (dir+1)%4);
                                loop_set(loop, n);
                            }
                        }
                        else {
                            program_terminated = true;
                        }
                    }
                }
                else {
                    alphanum *a1 = xmalloc(sizeof(alphanum));
                    if (!stack_empty(letnum_stack)){
                        alphanum *a = (alphanum*) stack_peek_n(letnum_stack, 0);
                        a1->i = a->i;
                        a1->is_char = a->is_char;
                    }
                    else {
                        a1->i = 0;
                        a1->is_char = false;
                    }
                    stack_push(letnum_stack, (void*) a1);
                }
                break;
            }

            /*Cube Mode: Does not interpret '_' as a command, instead reads it
            as a LetNum element
            Befunge Mode: Pops an int off the LetNum stack (n). Moves the cursor
            right if n=0, otherwise moves the cursor left*/
            case '_': {
                if (cube_mode){
                    alpha_num_symbol = true;
                }
                else {
                    int n = pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code == 0){
                        if (n == 0){
                            cursor_dir_set(cursor, 1);
                        }
                        else {
                            cursor_dir_set(cursor, 3);
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Does not interpret '\' as a command, instead reads it
            as a LetNum element
            Befunge Mode: Swaps top two characters on the LetNum stack*/
            case '\\': {
                if (cube_mode){
                    alpha_num_symbol = true;
                }
                else {
                    if (stack_size(letnum_stack) >= 2){
                        stack_swap_mn(letnum_stack, 0, 1);
                    }
                    else {
                        alphanum *a1 = xmalloc(sizeof(alphanum));
                        alphanum *a2 = xmalloc(sizeof(alphanum));
                        a1->i = 0;
                        a2->i = 0;
                        a1->is_char = false;
                        a2->is_char = false;
                        stack_push(letnum_stack, (void*) a1);
                        stack_push(letnum_stack, (void*) a2);
                    }
                }
                break;
            }

            /*Cube Mode: Does not interpret '.' as a command, instead reads it
            as a LetNum element
            Befunge Mode: Pops an int off the LetNum stack (n) and prints it as
            an integer followed by a space*/
            case '.': {
                if (cube_mode){
                    alpha_num_symbol = true;
                }
                else {
                    int n = pop_letnum_int(letnum_stack, &exit_code, false);
                    if (exit_code == 0){
                        printf("%d ", n);
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Does not interpret 'p' as a command, instead reads it
            as a LetNum element
            Befunge Mode: Pops 3 ints off the LetNum stack (y, x, v) and changes
            the character at (x, y) in the current tesseract face to the 
            character with ascii value v. Does nothing in the event (x, y) is
            out of bounds*/
            case 'p': {
                if (cube_mode){
                    alpha_num_symbol = true;
                }
                else {
                    int y = 0;
                    int x = 0;
                    exit_code = pop_2_letnum_ints(letnum_stack, &y, &x, false);
                    if (exit_code == 0){
                        int v = pop_letnum_int(letnum_stack, &exit_code, false);
                        if (exit_code == 0){
                            int len = (int) tesseract_length(T);
                            if (x < len && y < len){
                                tesseract_cell_write(T, cursor_get_cube(cursor),
                                                    cursor_get_square(cursor), 
                                                    (size_t) x, (size_t) y, 
                                                    (char) v);
                            }
                        }
                        else {
                            program_terminated = true;
                        }
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Cube Mode: Does not interpret 'g' as a command, instead reads it
            as a LetNum element
            Befunge Mode: Pops 2 ints off the LetNum stack (y, x) and pushes
            the ascii value of the character at (x, y) in the current tesseract 
            face to the LetNum stack. Pushes 0 in the event (x, y) is
            out of bounds*/
            case 'g': {
                if (cube_mode){
                    alpha_num_symbol = true;
                }
                else {
                    int y = 0;
                    int x = 0;
                    exit_code = pop_2_letnum_ints(letnum_stack, &y, &x, false);
                    if (exit_code == 0){
                        alphanum *a = xmalloc(sizeof(alphanum));
                        char c = tesseract_cell_read(T, cursor_get_cube(cursor),
                                                    cursor_get_square(cursor), 
                                                    (size_t) x, (size_t) y);
                        a->i = (int) c;
                        a->is_char = false;
                        stack_push(letnum_stack, (void*) a);
                    }
                    else {
                        program_terminated = true;
                    }
                }
                break;
            }

            /*Both Modes: Moves the cursor to the cube face corresponding to the 
            cusor's current cube face in 4D and changes x direction if 
            applicable. Additionally toggles the language mode from cube to 
            Befunge and vice versa*/
            case '{': {
                cursor_wormhole(cursor);
                cube_mode = !cube_mode;
                break;
            }

            /*Both Modes: Flexes on Cube*/
            case '}': {
                printf("Tesseract: 8x better than Cube!\n");
                break;
            }
            
            /*If the read character is not one of the listed commands, interpret
            it as a LetNum element*/
            default: {
                alpha_num_symbol = true;
                break;
            }
        }

        if (alpha_num_symbol){
            alpha_num_symbol = false;
            alphanum *a;
            if (isdigit(symbol)){
                a = xmalloc(sizeof(alphanum));
                a->i = symbol - '0';
                a->is_char = false;
                stack_push(letnum_stack, (void*) a);
            }
            /*If the read character isn't a command and not a number while in 
            Befunge mode, ignore it*/
            if (!isdigit(symbol) && cube_mode){
                a = xmalloc(sizeof(alphanum));
                a->i = symbol;
                a->is_char = true;
                stack_push(letnum_stack, (void*) a);
            }
        }
    }
    IF_DEBUG(print_debug(T, letnum_stack, operation_stack,
                             true, false, false, false, false, false, false));

    stack_free(letnum_stack, &free_alphanum);
    stack_free(operation_stack, &free_operation);
    tesseract_free(T);

    if (exit_code != 0){
        errorHandler(exit_code);
    }
    return EXIT_SUCCESS;
}
