#include <stdlib.h>
#include <stdbool.h>
#include "tesseracts.h"

#ifndef READ_SCRIPT_H
#define READ_SCRIPT_H

typedef struct script_header script;

tesseract_t read_script(const char *filename);
/*requires strlen(filename) > 0 && strlen(filename) <= 256*/
/*ensures result != NULL*/

script* filename_to_script(const char *filename);

void free_script(script *S);

char **get_script_data(script *S);

#endif
