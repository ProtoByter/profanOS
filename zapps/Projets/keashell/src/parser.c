#include <syscall.h>
#include "lexer.h"
#include "parser.h"

ParsedProgram_t run_parser(LexedProgram_t program) {
    // create the instructions list
    Instruction_t *instructions = NULL;
    Instruction_t *current_instruction = NULL;
    // parse the program (crea)
    for (int i = 0; i < program.size; i++) {
        // if the word is a number
        if (program.words[i].type == W_NUMBER) {
            // create the element
            ParserElement_t *element = (ParserElement_t *) c_malloc(sizeof(ParserElement_t));
            element->data_type = E_NUMBER;
            element->data_int = c_ascii_to_int(program.words[i].word);
            element->next = NULL;
            // create the instruction
            Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
            instruction->type = I_PUSH;
            instruction->element = element;
            instruction->next = NULL;
            // add the instruction to the list
            if (instructions == NULL) {
                instructions = instruction;
            } else {
                current_instruction->next = instruction;
            }
            current_instruction = instruction;
        } 
        // if the word is a string
        else if (program.words[i].type == W_STRING) {
            // create the element
            ParserElement_t *element = (ParserElement_t *) c_malloc(sizeof(ParserElement_t));
            element->data_type = E_STRING;
            element->data_str = program.words[i].word;
            element->next = NULL;
            // create the instruction
            Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
            instruction->type = I_PUSH;
            instruction->element = element;
            instruction->next = NULL;
            // add the instruction to the list
            if (instructions == NULL) {
                instructions = instruction;
            } else {
                current_instruction->next = instruction;
            }
            current_instruction = instruction;
        }
        // if the word is a command
        else {
            // if the command is +
            if (c_str_cmp(program.words[i].word, "+") == 0) {
                // create the instruction
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_ADD;
                instruction->element = NULL;
                instruction->next = NULL;
                // add the instruction to the list
                if (instructions == NULL) {
                    instructions = instruction;
                } else {
                    current_instruction->next = instruction;
                }
                current_instruction = instruction;
            }
            // if the command is .
            if (c_str_cmp(program.words[i].word, ".") == 0) {
                // create the instruction
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_PRINT;
                instruction->element = NULL;
                instruction->next = NULL;
                // add the instruction to the list
                if (instructions == NULL) {
                    instructions = instruction;
                } else {
                    current_instruction->next = instruction;
                }
                current_instruction = instruction;
            }
        }
    }

    // free the memory allocated by the lexer
    for (int i = 0; i < program.size; i++) {
        c_free(program.words[i].word);
    }
    c_free(program.words);

    // create the parsed program
    ParsedProgram_t parsed_program;
    parsed_program.instructions = instructions;

    return parsed_program;
}

