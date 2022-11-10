#ifndef KEASHELL_INTERPRETOR_H
#define KEASHELL_INTERPRETOR_H

#include "parser.h"

typedef enum ErrorCodes_t {
    NO_ERROR = 0,
    ERROR_CANNOT_ADD_TWO_ELEMENTS_WHO_ARE_NOT_OF_THE_SAME_TYPE = 1,
    ERROR_STACK_UNDERFLOW = 2,
    ERROR_STACK_OVERFLOW = 3,
    ERROR_INVALID_TYPE_PRINT = 4,
    ERROR_NOT_RECHEABLE = 5,
} ErrorCodes_t;

typedef struct InterpretorElement_t {
    int data_type;
    int data_int;
    char *data_str;
} InterpretorElement_t;

typedef struct ElementStack_t {
    InterpretorElement_t *element_list;
    int size;
    int top_index;
} ElementStack_t;

int run_interpretor(ParsedProgram_t program);

#endif 