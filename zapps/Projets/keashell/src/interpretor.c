#include <syscall.h>

#include "interpretor.h"

int run_interpretor(ParsedProgram_t program) {
    // TODO: free the memory allocated by the parser

    // create the stack
    int STACK_SIZE = 100;
    ElementStack_t stack;
    stack.element_list = (InterpretorElement_t *) c_malloc(sizeof(InterpretorElement_t)*STACK_SIZE);
    stack.size = STACK_SIZE;
    stack.top_index = -1;

    // store the error code
    int error_code = NO_ERROR;

    while (program.instructions != NULL) {
        // shouldn't happend, but in case i forgor to add a break after an error
        if (error_code != NO_ERROR) {
            break;
        }

        if (program.instructions->type == I_PUSH) {
            // if the stack is full
            if (stack.top_index == stack.size - 1) {
                error_code = ERROR_STACK_OVERFLOW;
                // special case : we need to free the memory allocated by the parser
                c_free(program.instructions->element);
                break;
            }
            // add the element to the stack
            stack.top_index++;
            // new element beacause we change types
            InterpretorElement_t element;
            element.data_type = program.instructions->element->data_type;
            element.data_int = program.instructions->element->data_int;
            if (program.instructions->element->data_type == E_STRING) {
                element.data_str = (char *) c_malloc(sizeof(char)*c_str_len(program.instructions->element->data_str));
                c_str_cpy(element.data_str, program.instructions->element->data_str);
            } else {
                element.data_str = NULL;
            }
            stack.element_list[stack.top_index] = element;
            // free the memory allocated by the parser
            c_free(program.instructions->element);
        }
        else if (program.instructions->type == I_ADD) {
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
        else if (program.instructions->type == I_PRINT) {
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

        // go to the next instruction
        void *to_free = (void *) program.instructions;
        program.instructions = program.instructions->next;
        c_free(to_free);
    }

    // if there is an error code, we need to free program.instructions
    if (error_code != NO_ERROR) {
        while (program.instructions != NULL) {
            // free the memory allocated by the parser
            c_free(program.instructions->element);
            void *to_free = (void *) program.instructions;
            program.instructions = program.instructions->next;
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
    c_free(program.instructions);

    return error_code;
}