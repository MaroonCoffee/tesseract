#include <stdlib.h>
#include <stdbool.h>
#include "tesseracts.h"

#ifndef READ_SCRIPT_H
#define READ_SCRIPT_H

tesseract_t read_script(const char *filename);
/*requires strlen(filename) > 0 && strlen(filename) <= 256*/
/*ensures result != NULL*/

#endif
