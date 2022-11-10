#ifndef KEASHELL_INTERPRETOR_H
#define KEASHELL_INTERPRETOR_H

#include "parser.h"
#include "typecheck.h"

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