#ifndef KEASHELL_PARSER_H
#define KEASHELL_PARSER_H

#include "lexer.h"

typedef enum InstructionDataType_t {
    E_NUMBER,
    E_STRING
} InstructionDataType_t;

typedef enum Instructions_types_t {
    I_PUSH,
    I_ADD,
    I_PRINT,
} Instructions_types_t;

typedef struct ParserElement_t {
    int data_type;
    int data_int;
    char *data_str;
    struct ParserElement_t *next;
} ParserElement_t;

typedef struct Instruction_t {
    Instructions_types_t type;
    ParserElement_t *element;
    struct Instruction_t *next;
} Instruction_t;

typedef struct ParsedProgram_t {
    Instruction_t *instructions;
} ParsedProgram_t;

ParsedProgram_t run_parser(LexedProgram_t program);

#endif 