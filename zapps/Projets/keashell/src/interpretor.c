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

    while (program.instructions != NULL) {
        if (program.instructions->type == I_PUSH) {
            // if the stack is full
            if (stack.top_index == stack.size - 1) {
                return ERROR_STACK_OVERFLOW;
            }
            // add the element to the stack
            stack.top_index++;
            // new element beacause we change types
            InterpretorElement_t *element = (InterpretorElement_t *) c_malloc(sizeof(InterpretorElement_t));
            element->data_type = program.instructions->element->data_type;
            element->data_int = program.instructions->element->data_int;
            element->data_str = program.instructions->element->data_str;
            stack.element_list[stack.top_index] = *element;
        }
        else if (program.instructions->type == I_ADD) {
            if (stack.top_index < 1) {
                return ERROR_STACK_UNDERFLOW;
            }
            // if the two elements are numbers
            if (stack.element_list[stack.top_index].data_type == E_NUMBER && stack.element_list[stack.top_index - 1].data_type == E_NUMBER) {
                // new element
                InterpretorElement_t element;
                element.data_type = E_NUMBER;
                element.data_int = stack.element_list[stack.top_index].data_int + stack.element_list[stack.top_index - 1].data_int;
                // remove the two elements from the stack
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
                element.data_str = (char *) c_malloc(sizeof(char)*size);
                c_str_cpy(element.data_str, stack.element_list[stack.top_index - 1].data_str);
                c_str_cat(element.data_str, stack.element_list[stack.top_index].data_str);
                element.data_str[size - 1] = '\0';
                // remove the two elements from the stack
                stack.top_index -= 2;
                // add the new element to the stack
                stack.top_index++;
                stack.element_list[stack.top_index] = element;
            }
            else {
                return ERROR_CANNOT_ADD_TWO_ELEMENTS_WHO_ARE_NOT_OF_THE_SAME_TYPE;
            }
        }
        else if (program.instructions->type == I_PRINT) {
            if (stack.top_index == -1) {
                return ERROR_STACK_UNDERFLOW;
            }
            // if the element is a number
            if (stack.element_list[stack.top_index].data_type == E_NUMBER) {
                c_fskprint("%d", stack.element_list[stack.top_index].data_int);
            }
            // if the element is a string
            else if (stack.element_list[stack.top_index].data_type == E_STRING) {
                c_fskprint("%s", stack.element_list[stack.top_index].data_str);
            }
            else {
                return ERROR_INVALID_TYPE_PRINT;
            }
        }

        // go to the next instruction
        program.instructions = program.instructions->next;
    }


    return NO_ERROR;
}