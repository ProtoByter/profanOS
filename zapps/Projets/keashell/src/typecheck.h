#ifndef KEASHELL_TYPECHECK_H
#define KEASHELL_TYPECHECK_H

#include "parser.h"
#include "lexer.h"

// TODO : make error codes a typedef struct { int code; char *message; } ErrorCode_t;
typedef enum ErrorCodes_t {
    NO_ERROR = 0,
    ERROR_CANNOT_ADD_TWO_ELEMENTS_WHO_ARE_NOT_OF_THE_SAME_TYPE = 1,
    ERROR_STACK_UNDERFLOW = 2,
    ERROR_STACK_OVERFLOW = 3,
    ERROR_INVALID_TYPE_PRINT = 4,
    ERROR_NOT_RECHEABLE = 5,
    ERROR_STACK_NOT_EMPTY = 6,
} ErrorCodes_t;

// a type stack
typedef struct TypeStack_t {
    int top_index;
    int max_size;
    InstructionDataType_t *element_list;
} TypeStack_t;

int run_typecheck(ParsedProgram_t Program);

#endif 