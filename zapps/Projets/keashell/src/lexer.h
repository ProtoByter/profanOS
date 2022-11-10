#ifndef KEASHELL_LEXER_H
#define KEASHELL_LEXER_H

typedef enum WordType_t {
    W_NUMBER,
    W_STRING,
    W_COMMAND
} WordType_t;

// TODO : add positionning
typedef struct Word_t {
    WordType_t type;
    char *word;
} Word_t;

typedef struct LexedProgram_t {
    int size; // number of words
    Word_t *words;
} LexedProgram_t;

LexedProgram_t run_lexer(char *code);

#endif 