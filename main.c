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
//requires e != NULL
{
    REQUIRES(e != NULL);
    alphanum *a = (alphanum*) e;
    free(a);
}

void free_operation(void *e)
//requires e != NULL
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

void print_letnum_elem(genstack_elem e)
{
    alphanum *a = (alphanum*) e;
    if (a->is_char){
        printf("%c", a->i);
    }
    else {
        printf("%d", a->i);
    }
}

void print_op_elem(genstack_elem e)
{
    char c = *((char*) e);
    printf("%c\n", c);
}

void print_debug(tesseract_t T, genstack_t ln_stack, genstack_t op_stack,
                 bool pt, bool op_on_wrap, bool ln_stack_m, bool print_cursor)
//requires T != NULL
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
    print_n_dashes(dash_len);
}

int pop_letnum_int(genstack_t letnum_stack, size_t *error)
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
            //Expected int but found char on LetNum stack!
            *error = 7;
        }
    }
    else {
        //Attempted to pop off empty LetNum stack!
        *error = 6;
    }
    return -1;
}

size_t pop_2_letnum_ints(genstack_t letnum_stack, int *i, int *j)
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
            //Expected int but found char on LetNum stack!
            return 7;
        }
    }
    else {
        //Attempted to pop off empty LetNum stack!
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
    //checks to see if 2 arguments were passed in
    if (argc > 2){
        //Too many arguments passed in!
        errorHandler(1);
    }
    if (argc < 2){
        //Too many arguments passed in!
        errorHandler(1);
    }
    ASSERT(argc == 2);
    
    //checks to see if filename is valid
    size_t len = strlen(argv[1]);
    if (len >= MAX_FILENAME_LENGTH){
        //Filename is too long!
        errorHandler(2);
    }
    if (argv[1][0] == ' '){
        //Invalid filename!
        errorHandler(3);
    }
    for (size_t i=0; argv[1][i] != '\0'; i++){
        char c = argv[1][i];
        if (!isalnum(c) && !(c == '_') && !(c == '-') && !(c == ',')
            && !(c == '.') && !(c == '(') && !(c == ')') && !(c == ' ')
            && !(c == '/')){
            //Invalid filename!
            errorHandler(3);
        }
    }
    if (len < 4 || (!is_file_extension(".tes", argv) && 
                    !is_file_extension(".cube", argv) && 
                    !is_file_extension(".dewy", argv))) {
        //Invalid file extension!
        errorHandler(4);
    }

    //checks to see that file exists with name
    if (access(argv[1], F_OK) == -1){
        //File does not exist!
        errorHandler(5);
    }

    return true;
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
    genstack_t letnum_stack = stack_new();
    genstack_t operation_stack = stack_new();

    while (!program_terminated){
        bool side_switch = cursor_move_next(tesseract_cursor(T));
        char symbol = tesseract_cursor_read(T);
        bool alpha_num_symbol = false;

        if (side_switch){
            side_switch = false;
            if (operation_on_wrap){
                if (!stack_empty(operation_stack)){
                    char *op_pointer = (char*) stack_pop(operation_stack);
                    char operation = *op_pointer;
                    free_operation((void*) op_pointer);
                    int i = 0;
                    int j = 0;
                    size_t error = pop_2_letnum_ints(letnum_stack, &i, &j);
                    if (error == 0){
                        switch (operation) {
                            case '+': {
                                int res = i+j;
                                alphanum *a = xmalloc(sizeof(alphanum));
                                a->i = res;
                                a->is_char = false;
                                stack_push(letnum_stack, (void*) a);
                                break;
                            }

                            case '-': {
                                int res = i-j;
                                alphanum *a = xmalloc(sizeof(alphanum));
                                a->i = res;
                                a->is_char = false;
                                stack_push(letnum_stack, (void*) a);
                                break;
                            }

                            case '*': {
                                int res = i*j;
                                alphanum *a = xmalloc(sizeof(alphanum));
                                a->i = res;
                                a->is_char = false;
                                stack_push(letnum_stack, (void*) a);
                                break;
                            }

                            case '/': {
                                int res = i/j;
                                alphanum *a = xmalloc(sizeof(alphanum));
                                a->i = res;
                                a->is_char = false;
                                stack_push(letnum_stack, (void*) a);
                                break;
                            }
                            
                            case '%': {
                                int res = i%j;
                                alphanum *a = xmalloc(sizeof(alphanum));
                                a->i = res;
                                a->is_char = false;
                                stack_push(letnum_stack, (void*) a);
                                break;
                            }

                            default: {
                                //Attempted to parse unknown operation!
                                exit_code = 11;
                                program_terminated = true;
                                break;
                            }
                        }
                    }
                    else {
                        exit_code = error;
                        program_terminated = true;
                    }
                }
                else {
                    //Attempted to pop off empty operation stack!
                    exit_code = 9;
                    program_terminated = true;
                }
            }
        }

        if (program_terminated){
            break;
        }

        IF_DEBUG(print_debug(T, letnum_stack, operation_stack,
                             program_terminated, operation_on_wrap, 
                             letnum_stack_mode, true));

        switch(symbol) {
            case EMPTY_CHAR: {
                break;
            }
            
            case '(': {
                break;
            }
                
            case ')': {
                program_terminated = true;
                break;
            }

            case '^': {
                cursor_dir_set(cursor, 0);
                break;
            }

            case '>': {
                cursor_dir_set(cursor, 1);
                break;
            }

            case 'v': {
                cursor_dir_set(cursor, 2);
                break;
            }

            case '<': {
                cursor_dir_set(cursor, 3);
                break;
            }

            case '+': {
                char *operation = xmalloc(sizeof(char));
                *operation = '+';
                stack_push(operation_stack, (void*) operation);
                break;
            }

            case '-': {
                char *operation = xmalloc(sizeof(char));
                *operation = '-';
                stack_push(operation_stack, (void*) operation);
                break;
            }

            case '*': {
                char *operation = xmalloc(sizeof(char));
                *operation = '*';
                stack_push(operation_stack, (void*) operation);
                break;
            }

            case '/': {
                char *operation = xmalloc(sizeof(char));
                *operation = '/';
                stack_push(operation_stack, (void*) operation);
                break;
            }

            case '%': {
                char *operation = xmalloc(sizeof(char));
                *operation = '%';
                stack_push(operation_stack, (void*) operation);
                break;
            }

            case '~': {
                alphanum *a = xmalloc(sizeof(alphanum));
                a->i = ' ';
                a->is_char = true;
                stack_push(letnum_stack, (void*) a);
                break;
            }

            case '"': {
                operation_on_wrap = !operation_on_wrap;
                break;
            }

            case '|': {
                printf("Hello World\n");
                break;
            }

            case ';': {
                size_t error = 0;
                int n = pop_letnum_int(letnum_stack, &error);
                if (error == 0){
                    if (n <= (int) stack_size(letnum_stack)){
                            for (int i=0; i<n; i++){
                                void *e = stack_pop(letnum_stack);
                                print_letnum_elem(e);
                                free_alphanum(e);
                            }
                            printf("\n");
                        }
                        else {
                            //Attempted to pop off empty LetNum stack!
                            exit_code = 6;
                            program_terminated = true;
                        }
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }

            case ',': {
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
                    //Error reading input!
                    exit_code = 8;
                    program_terminated = true;
                }
                break;
            }
            
            case '`': {
                stack_free(letnum_stack, &free_alphanum);
                stack_free(operation_stack, &free_operation);
                letnum_stack = stack_new();
                operation_stack = stack_new();
                break;
            }
            
            case '=': {
                size_t error = 0;
                int i = pop_letnum_int(letnum_stack, &error);
                if (error == 0){
                    alphanum *a = xmalloc(sizeof(alphanum));
                    a->i = i;
                    a->is_char = true;
                    stack_push(letnum_stack, (void*) a);
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }
            
            case '!': {
                letnum_stack_mode = !letnum_stack_mode;
                break;
            }
            
            case '#': {
                if (letnum_stack_mode){
                    if (!stack_empty(letnum_stack)){
                        alphanum *a = (alphanum*) stack_peek_n(letnum_stack, 0);
                        alphanum *a1 = xmalloc(sizeof(alphanum));
                        a1->i = a->i;
                        a1->is_char = a->is_char;
                        stack_push(letnum_stack, (void*) a1);
                    }
                    else {
                        //Attempted to pop off empty LetNum stack!
                        exit_code = 6;
                        program_terminated = true;
                    }
                }
                else {
                    if (!stack_empty(operation_stack)){
                        char c = *((char*) stack_peek_n(operation_stack, 0));
                        char *op = xmalloc(sizeof(char));
                        *op = c;
                        stack_push(operation_stack, (void*) op);
                    }
                    else {
                        //Attempted to pop off empty operation stack!
                        exit_code = 9;
                        program_terminated = true;
                    }
                }
                break;
            }
            
            case '$': {
                size_t error = 0;
                int n = pop_letnum_int(letnum_stack, &error);
                if (error == 0){
                    if (letnum_stack_mode){
                        if (n-1 < (int) stack_size(letnum_stack) && n-1>=0){
                            alphanum *a = (alphanum*) stack_peek_n(letnum_stack, 
                                                                   n-1);
                            alphanum *a1 = xmalloc(sizeof(alphanum));
                            a1->i = a->i;
                            a1->is_char = a->is_char;
                            stack_push(letnum_stack, (void*) a1);
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                    else {
                        if (n-1 < (int) stack_size(operation_stack) && n-1>=0){
                            char c = *((char*) stack_peek_n(operation_stack,
                                                            n-1));
                            char *op = xmalloc(sizeof(char));
                            *op = c;
                            stack_push(operation_stack, (void*) op);
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }
            
            case '&': {
                size_t error = 0;
                int n = pop_letnum_int(letnum_stack, &error);
                if (error == 0){
                    if (letnum_stack_mode){
                        if (n-1 < (int) stack_size(letnum_stack) && n-1>=0){
                            free_alphanum(stack_pop_n(letnum_stack, n-1));
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                    else {
                        if (n-1 < (int) stack_size(operation_stack) && n-1>=0){
                            free_operation(stack_pop_n(operation_stack, n-1));
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }
            
            case '@': {
                int i = 0;
                int j = 0;
                size_t error = pop_2_letnum_ints(letnum_stack, &i, &j);
                if (error == 0){
                    if (letnum_stack_mode){
                        if (i-1 < (int) stack_size(letnum_stack) && i-1>= 0 &&
                            j-1 < (int) stack_size(letnum_stack) && j-1>= 0)
                        {
                            stack_swap_mn(letnum_stack, i-1, j-1);
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                    else {
                        if (i-1 < (int) stack_size(operation_stack) && i-1>=0 &&
                            j-1 < (int) stack_size(operation_stack) && j-1>=0)
                        {
                            stack_swap_mn(operation_stack, i-1, j-1);
                        }
                        else {
                            //Stack index out of bounds!
                            exit_code = 10;
                            program_terminated = true;
                        }
                    }
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }
            
            case '?': {
                int i = 0;
                int j = 0;
                size_t error = pop_2_letnum_ints(letnum_stack, &i, &j);
                if (error == 0){
                    size_t dir = cursor_get_dir(cursor);
                    if (i > j){
                        cursor_dir_set(cursor, (dir-1)%4);
                    }
                    else if (i < j){
                        cursor_dir_set(cursor, (dir+1)%4);
                    }
                }
                else {
                    exit_code = error;
                    program_terminated = true;
                }
                break;
            }
            
            case ':': {
                size_t dir = cursor_get_dir(cursor);
                int key = loop_key(cursor_get_cube(cursor), 
                                   cursor_get_square(cursor), 
                                   cursor_get_x(cursor), cursor_get_y(cursor), 
                                   tesseract_length(T));
                loop_t loop = loop_dict_lookup(tesseract_get_loop_dict(T), key);
                if (loop == NULL){
                    loop_init(tesseract_get_loop_dict(T), key);
                    loop = loop_dict_lookup(tesseract_get_loop_dict(T), key);
                }
                ASSERT(loop != NULL);
                if (is_loop_active(loop)){
                    loop_decrease(loop);
                    if (is_loop_active(loop)){
                        cursor_dir_set(cursor, (dir+1)%4);
                    }
                }
                else{
                    size_t error = 0;
                    int n = pop_letnum_int(letnum_stack, &error);
                    if (error == 0){
                        if (n > 0){
                            cursor_dir_set(cursor, (dir+1)%4);
                            loop_set(loop, n);
                        }
                    }
                    else {
                        exit_code = error;
                        program_terminated = true;
                    }
                }
                break;
            }

            case '{': {
                cursor_wormhole(cursor);
                break;
            }
            
            default: {
                alpha_num_symbol = true;
                break;
            }
        }

        if (alpha_num_symbol){
            alpha_num_symbol = false;
            alphanum *a = xmalloc(sizeof(alphanum));
            if (isdigit(symbol)){
                a->i = symbol - '0';
                a->is_char = false;
            }
            else{
                a->i = symbol;
                a->is_char = true;
            }
            stack_push(letnum_stack, (void*) a);
        }
    }
    IF_DEBUG(print_debug(T, letnum_stack, operation_stack,
                             true, false, false, false));

    stack_free(letnum_stack, &free_alphanum);
    stack_free(operation_stack, &free_operation);
    tesseract_free(T);

    if (exit_code != 0){
        errorHandler(exit_code);
    }
    return EXIT_SUCCESS;
}

//Three file extensions: .tes .cube .dewy (switching modes based on type)
//Cube files read in with special function and in center cube
//Implement the following instructions
// ( : Begins execution at this point
// ) : Ends execution at this point
// > : Turn right
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
