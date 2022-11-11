#include <syscall.h>
#include "lexer.h"
#include "parser.h"
#include "typecheck.h"
#include "settings.h"

int run(Instruction_t *Program_instructions, Settings_t settings) {
    // TODO : show the user at what line and character the error occured

    if (settings.flags & FLAG_NO_TYPECHECK) {
        return NO_ERROR;
    }

    ErrorCodes_t error_code = NO_ERROR;

    // the type stack
    TypeStack_t type_stack;
    type_stack.top_index = 0;
    type_stack.max_size = 100;
    type_stack.element_list = (InstructionDataType_t *) c_malloc(sizeof(InstructionDataType_t) * type_stack.max_size);

    // temp variable to not channge the program
    Instruction_t *current_instruction = Program_instructions;

    // we loop on every instruction
    while (current_instruction != NULL) {
        // show the instruction
        // c_fskprint("Instruction : %d, ", current_instruction->type);

        if (error_code != NO_ERROR) {
            // if there is an error, we stop the typecheck there
            break;
        }

        // if the instruction is a push
        if (current_instruction->type == I_PUSH) {
            // if the element is a number
            if (current_instruction->element->data_type == E_NUMBER) {
                // we push the type of the instruction
                type_stack.element_list[type_stack.top_index] = E_NUMBER;
                type_stack.top_index++;
            }
            // if the element is a string
            else if (current_instruction->element->data_type == E_STRING) {
                // we push the type of the instruction
                type_stack.element_list[type_stack.top_index] = E_STRING;
                type_stack.top_index++;
            }
        }
        // if the instruction is an add
        else if (current_instruction->type == I_ADD) {
            // if the stack underflow
            if (type_stack.top_index < 2) {
                error_code = ERROR_STACK_UNDERFLOW;
            }
            // if the two elements are not of the same type
            if (type_stack.element_list[type_stack.top_index - 1] != type_stack.element_list[type_stack.top_index - 2]) {
                error_code = ERROR_CANNOT_ADD_TWO_ELEMENTS_WHO_ARE_NOT_OF_THE_SAME_TYPE;
            }
            // we pop the type of the instruction
            type_stack.top_index--;
        }
        // if the instruction is a print
        else if (current_instruction->type == I_PRINT) {
            // if the stack underflow
            if (type_stack.top_index < 1) {
                error_code = ERROR_STACK_UNDERFLOW;
            }
            // we pop the type of the instruction
            type_stack.top_index--;
        }

        // if the instruction is unknown
        else if (current_instruction->type == I_UNKNOWN) {
            error_code = ERROR_UNKNOWN_INSTRUCTION;
        }

        // we go to the next instruction
        current_instruction = current_instruction->next_instruction;
    }

    // if the stack is not empty
    if (type_stack.top_index != 0) {
        error_code = ERROR_STACK_NOT_EMPTY;
    }

    // we free the memory allocated by the type stack
    c_free(type_stack.element_list);

    return error_code;
}

int run_typecheck(ParsedProgram_t Program, Settings_t settings) {
    return run(Program->instructions, settings);
}