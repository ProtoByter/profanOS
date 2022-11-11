#include <syscall.h>

#include "interpretor.h"
#include "settings.h"

int runI(Instruction_t *program_instructions, Settings_t settings) {

    if (settings.flags & FLAG_NO_INTERPRETOR) {
        return NO_ERROR;
    }

    // create the stack
    int STACK_SIZE = 100;
    ElementStack_t stack;
    stack.element_list = (InterpretorElement_t *) c_malloc(sizeof(InterpretorElement_t)*STACK_SIZE);
    stack.size = STACK_SIZE;
    stack.top_index = -1;

    // store the error code
    int error_code = NO_ERROR;

    while (program_instructions != NULL) {
        // shouldn't happend, but in case i forgor to add a break after an error
        if (error_code != NO_ERROR) {
            error_code = ERROR_NOT_RECHEABLE;
            break;
        }

        if (program_instructions->type == I_PUSH) {
            // if the stack is full
            if (stack.top_index == stack.size - 1) {
                error_code = ERROR_STACK_OVERFLOW;
                // special case : we need to free the memory allocated by the parser
                c_free(program_instructions->element);
                break;
            }
            // add the element to the stack
            stack.top_index++;
            // new element beacause we change types
            InterpretorElement_t element;
            element.data_type = program_instructions->element->data_type;
            element.data_int = program_instructions->element->data_int;
            if (program_instructions->element->data_type == E_STRING) {
                element.data_str = (char *) c_malloc(sizeof(char)*c_str_len(program_instructions->element->data_str));
                c_str_cpy(element.data_str, program_instructions->element->data_str);
            } else {
                element.data_str = NULL;
            }
            stack.element_list[stack.top_index] = element;
            // free the memory allocated by the parser
            c_free(program_instructions->element);
        }
        else if (program_instructions->type == I_ADD) {
            if (stack.top_index < 1) {
                error_code =  ERROR_STACK_UNDERFLOW;
                break;
            }
            // if the two elements are numbers
            if (stack.element_list[stack.top_index].data_type == E_NUMBER && stack.element_list[stack.top_index - 1].data_type == E_NUMBER) {
                // new element
                InterpretorElement_t element;
                element.data_type = E_NUMBER;
                element.data_int = stack.element_list[stack.top_index].data_int + stack.element_list[stack.top_index - 1].data_int;
                stack.top_index -= 2;
                // add the new element to the stack
                stack.top_index++;
                stack.element_list[stack.top_index] = element;
            }
            // if the two elements are strings
            else if (stack.element_list[stack.top_index].data_type == E_STRING && stack.element_list[stack.top_index - 1].data_type == E_STRING) {
                // new element
                InterpretorElement_t element;
                element.data_type = E_STRING;

                int size = c_str_len(stack.element_list[stack.top_index].data_str) + c_str_len(stack.element_list[stack.top_index - 1].data_str) + 1;
                element.data_str = (char *) c_calloc(sizeof(char)*size);
                c_str_cpy(element.data_str, stack.element_list[stack.top_index - 1].data_str);
                c_str_cat(element.data_str, stack.element_list[stack.top_index].data_str);
                element.data_str[size - 1] = '\0';
                // remove the two elements from the stack (we need to free them beacause they are strings)
                for (int i = 0; i < 2; i++) {
                    c_free(stack.element_list[stack.top_index].data_str);
                    stack.top_index--;
                }
                // add the new element to the stack
                stack.top_index++;
                stack.element_list[stack.top_index] = element;
            }
            else {
                error_code = ERROR_CANNOT_ADD_TWO_ELEMENTS_WHO_ARE_NOT_OF_THE_SAME_TYPE;
                break;
            }
        }
        else if (program_instructions->type == I_PRINT) {
            if (stack.top_index < 0) {
                return ERROR_STACK_UNDERFLOW;
            }
            // if the element is a number
            if (stack.element_list[stack.top_index].data_type == E_NUMBER) {
                c_fskprint("%d", stack.element_list[stack.top_index].data_int);
                stack.top_index--;
            }
            // if the element is a string
            else if (stack.element_list[stack.top_index].data_type == E_STRING) {
                c_fskprint("%s", stack.element_list[stack.top_index].data_str);
                c_free(stack.element_list[stack.top_index].data_str);
                stack.top_index--;
            }
            else {
                error_code = ERROR_INVALID_TYPE_PRINT;
                break;
            }
        }
        else if (program_instructions->type == I_IF) {
            // if the stack is empty
            if (stack.top_index < 0) {
                error_code = ERROR_STACK_UNDERFLOW;
                break;
            }
            int condition = 0;
            // if the element is a number
            if (stack.element_list[stack.top_index].data_type == E_NUMBER) {
                condition = stack.element_list[stack.top_index].data_int;
                stack.top_index--;
            }
            // if the element is a string
            else if (stack.element_list[stack.top_index].data_type == E_STRING) {
                condition = c_str_len(stack.element_list[stack.top_index].data_str);
                c_free(stack.element_list[stack.top_index].data_str);
                stack.top_index--;
            }
            else {
                error_code = ERROR_INVALID_TYPE_IF;
                break;
            }
            // if the condition is true
            if (condition) {
                // we need to execute the instructions
                Instruction_t *temp_instructions = program_instructions->instruction_branch;
                runI(temp_instructions, settings);
            }
        }

        // if the instruction is unknown
        else if (program_instructions->type == I_UNKNOWN) {
            error_code = ERROR_UNKNOWN_INSTRUCTION;
            break;
        }

        // go to the next instruction
        void *to_free = (void *) program_instructions;
        program_instructions = program_instructions->next_instruction;
        c_free(to_free);
    }

    // if there is an error code, we need to free program_instructions
    if (error_code != NO_ERROR) {
        while (program_instructions != NULL) {
            // free the memory allocated by the parser
            c_free(program_instructions->element);
            void *to_free = (void *) program_instructions;
            program_instructions = program_instructions->next_instruction;
            c_free(to_free);
        }
    }

    while (stack.top_index >= 0) {
        if (stack.element_list[stack.top_index].data_type == E_STRING) {
            c_free(stack.element_list[stack.top_index].data_str);
        }
        stack.top_index--;
    }
    c_free(stack.element_list);
    c_free(program_instructions);

    return error_code;
}

int run_interpretor(ParsedProgram_t program, Settings_t settings) {
    Instruction_t *temp_instructions = program.instructions;
    return runI(temp_instructions, settings);
}