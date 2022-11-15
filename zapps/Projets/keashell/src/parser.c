#include <syscall.h>
#include "lexer.h"
#include "parser.h"
#include "settings.h"

void add_instruction(Instruction_t **instructions, Instruction_t **instruction, Instruction_t **current_instruction);
void print_parser_output(Instruction_t *instructions, int first);

ParsedProgram_t run_parser(LexedProgram_t program, Settings_t settings, int in_recursive_call) {
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
                // TODO : take into account ifs in ifs
                // TODO : add recursives ifs

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
                int if_counts = 1;
                int end_counts = 0;
                int size = 0;
                while (1) {
                    if (c_str_cmp(program.words[j].word, "if") == 0) {
                        if_counts++;
                    } else if (c_str_cmp(program.words[j].word, "end") == 0) {
                        end_counts++;
                    }
                    if (if_counts == end_counts) {
                        break;
                    }
                    size++;
                    j++;
                    // if we are at the end of the program
                    if (j >= program.size) {
                        // TODO : parser error
                        break;
                    }
                }
                // we alloc the words
                program_branch.words = (Word_t *) c_malloc(sizeof(Word_t) * size);
                for (int k = i + 1; k < j; k++) {
                    program_branch.words[program_branch.size] = program.words[k];
                    program_branch.size++;
                }
                
                // show the program (is this really needed ??)
                // if (settings.flags & FLAG_PRINT_PARSER_OUTPUT) {
                //     for (int k = 0; k < program_branch.size; k++) {
                //         c_fskprint("%s ", program_branch.words[k].word);
                //     } c_fskprint("\n");
                // }
                
                // parse the program
                ParsedProgram_t program_branch_parsed = run_parser(program_branch, settings, 1);
                // add the instruction branch to the instruction
                instruction->instruction_branch = program_branch_parsed.instructions;
                // update the index
                i = j;
            }
            // if the command is unknown
            else {
                // create the instruction and show an error message
                c_fskprint("Unknown command : %s\n", program.words[i].word);
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

    if (settings.flags & FLAG_PRINT_PARSER_OUTPUT && !in_recursive_call) {
        print_parser_output(parsed_program.instructions, 0);
    }

    return parsed_program;
}

void add_instruction(Instruction_t **instructions, Instruction_t **instruction, Instruction_t **current_instruction) {
    if (*instructions == NULL) {
        *instructions = *instruction;
    } else {
        (*current_instruction)->next_instruction = *instruction;
    }
    *current_instruction = *instruction;
}

void print_parser_output(Instruction_t *instructions, int first) {
    if (first == 0) {
        c_fskprint("Parser output:\n");
    }
    c_fskprint("(");
    Instruction_t *current_instruction = instructions;
    while (current_instruction != NULL) {
        switch (current_instruction->type) {
            case I_PUSH:
                if (current_instruction->element->data_type == E_NUMBER) {
                    c_fskprint(" PUSH(%d)", current_instruction->element->data_int);
                } else if (current_instruction->element->data_type == E_STRING) {
                    c_fskprint(" PUSH(\"%s\")", current_instruction->element->data_str);
                }
                break;
            case I_ADD:
                c_fskprint(" ADD");
                break;
            case I_PRINT:
                c_fskprint(" PRINT");
                break;
            case I_IF:
                c_fskprint(" IF");
                print_parser_output(current_instruction->instruction_branch, 1);
                break;
            case I_UNKNOWN:
                c_fskprint(" UNKNOWN(%d,%s,%d)", current_instruction->element->data_type, current_instruction->element->data_str, current_instruction->element->data_int);
                break;
        }
        current_instruction = current_instruction->next_instruction;
    }
    c_fskprint(" )");
    if (first == 0) {
        c_fskprint("\n");
    }
}
