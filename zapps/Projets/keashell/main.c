#include <syscall.h>
#include "src/lexer.h"
#include "src/parser.h" 
#include "src/interpretor.h"

int main(int argc, char **argv) {
    // current memory leak :
    // -Execution normale : DONE
    // -Execution avec erreur : DONE

    while (1) {

        char *code = (char *) c_malloc(sizeof(char) * 1000);
        c_fskprint("Keashell >>> ");
        c_input(code, 1000, c_blue);

        c_fskprint("\n");

        LexedProgram_t program = run_lexer(code);
        ParsedProgram_t parsed_program = run_parser(program);

        int typecheck_error_code = run_typecheck(parsed_program);

        if (typecheck_error_code) {
            c_fskprint("Error: typecheck failed with error code %d\n", typecheck_error_code);
        } else {
            int error_code = run_interpretor(parsed_program);
            if (error_code != NO_ERROR) {
                c_fskprint("Error: interpretor failed with error code %d\n", error_code);
            }
        }

    }

    while (1);
    return 0;
}
