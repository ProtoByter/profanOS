#include <syscall.h>
#include "lexer.h"
#include "settings.h"

int str_is_number(char *str);

LexedProgram_t run_lexer(char *code, Settings_t settings) { 
    LexedProgram_t program;

    // count the number of words
    program.size = 0;
    for (int i = 0; i < c_str_len(code); i++) {
        if (code[i] == ' ') {
            program.size++;
        }
    }
    program.size++;

    // allocate the words
    program.words = (Word_t *) c_malloc(sizeof(Word_t) * program.size);
    char *word = (char *) c_malloc(sizeof(char) * 100);
    int word_index = 0;
    int program_index = 0;

    // fill the words
    for (int i = 0; i < c_str_len(code); i++) {
        // TODO : handle the case where the word is a string with spaces
        if (code[i] == ' ') {
            word[word_index] = '\0';
            program.words[program_index].word = word;
            program_index++;
            word = (char *) c_malloc(sizeof(char) * 100);
            word_index = 0;
        } else {
            word[word_index] = code[i];
            word_index++;
        }
    }
    program.words[program_index].word = word;
    program.words[program_index].word[word_index] = '\0';

    // set the type of each word (number, string, command)
    for (int i = 0; i < program.size; i++) {
        // if the word is a number
        if (str_is_number(program.words[i].word)) {
            program.words[i].type = W_NUMBER;
        } 
        // if the word is a string starting and ending with ""
        else if (program.words[i].word[0] == '"' && program.words[i].word[c_str_len(program.words[i].word) - 1] == '"') {
            program.words[i].type = W_STRING;
            // remove the " at the beginning and the end of the string
            char *new_word = (char *) c_malloc(sizeof(char) * 100);
            for (int j = 1; j < c_str_len(program.words[i].word) - 1; j++) {
                new_word[j - 1] = program.words[i].word[j];
            }
            new_word[c_str_len(program.words[i].word) - 2] = '\0';
            c_str_cpy(program.words[i].word, new_word);
            c_free(new_word);      

            // TODO : go trough the string, replacing \N with a space, \n with a newline, \t with a tab, etc. (see https://en.wikipedia.org/wiki/Escape_sequences_in_C)
        }
        // if the word is a command
        else {
            program.words[i].type = W_COMMAND;
        }
    }
    
    return program;
}

int str_is_number(char *str) {
    for (int i = 0; i < c_str_len(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}