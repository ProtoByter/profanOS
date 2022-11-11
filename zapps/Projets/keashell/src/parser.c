#include <syscall.h>
#include "lexer.h"
#include "parser.h"
#include "settings.h"

void add_instruction(Instruction_t **instructions, Instruction_t **instruction, Instruction_t **current_instruction) {
    if (*instructions == NULL) {
        *instructions = *instruction;
    } else {
        (*current_instruction)->next_instruction = *instruction;
    }
    *current_instruction = *instruction;
}


ParsedProgram_t run_parser(LexedProgram_t program, Settings_t settings) {
    // create the instructions list
    Instruction_t *instructions = NULL;
    Instruction_t *current_instruction = NULL;
    // parse the program
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
            instruction->next_instruction = NULL;
            // add the instruction to the list
            add_instruction(&instructions, &instruction, &current_instruction);
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
            instruction->next_instruction = NULL;
            // add the instruction to the list
            add_instruction(&instructions, &instruction, &current_instruction);
        }
        // if the word is a command
        else {
            // if the command is +
            if (c_str_cmp(program.words[i].word, "+") == 0) {
                // create the instruction
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_ADD;
                instruction->element = NULL;
                instruction->next_instruction = NULL;
                // add the instruction to the list
                add_instruction(&instructions, &instruction, &current_instruction);
            }
            // if the command is .
            else if (c_str_cmp(program.words[i].word, ".") == 0) {
                // create the instruction
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_PRINT;
                instruction->element = NULL;
                instruction->next_instruction = NULL;
                // add the instruction to the list
                add_instruction(&instructions, &instruction, &current_instruction);
            }
            // if the command is an if
            else if (c_str_cmp(program.words[i].word, "if") == 0) {
                // special case beacause we need to put in instruction.instruction_branch everything until the end
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_IF;
                instruction->element = NULL;
                instruction->next_instruction = NULL;
                // add the instruction to the list
                add_instruction(&instructions, &instruction, &current_instruction);
                // create the instruction branch
                Instruction_t *instruction_branch = NULL;
                Instruction_t *current_instruction_branch = NULL;
                // parse the program (recursive)
                // make a LexedProgram_t with the words between the if and the end
                LexedProgram_t program_branch;
                program_branch.size = 0;
                program_branch.words = NULL;
                int j = i + 1;
                // we get the size we need to alloc
                int size = 0;
                while (c_str_cmp(program.words[j].word, "end") != 0) {
                    size++;
                    j++;
                }
                // we alloc the words
                program_branch.words = (Word_t *) c_malloc(sizeof(Word_t) * size);
                // we copy the words
                j = i + 1;
                while (c_str_cmp(program.words[j].word, "end") != 0) {
                    program_branch.words[program_branch.size] = program.words[j];
                    program_branch.size++;
                    j++;
                }
                // parse the program
                ParsedProgram_t program_branch_parsed = run_parser(program_branch, settings);
                // add the instruction branch to the instruction
                instruction->instruction_branch = program_branch_parsed.instructions;
                // update the index
                i = j;
            }
            // if the command is unknown
            else {
                // create the instruction
                Instruction_t *instruction = (Instruction_t *) c_malloc(sizeof(Instruction_t));
                instruction->type = I_UNKNOWN;
                instruction->element = NULL;
                instruction->next_instruction = NULL;
                // add the instruction to the list
                add_instruction(&instructions, &instruction, &current_instruction);
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
