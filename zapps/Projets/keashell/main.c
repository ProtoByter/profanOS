#include <syscall.h>
#include "src/lexer.h"
#include "src/parser.h" 
#include "src/interpretor.h"

int main(int argc, char **argv) {
    // current memory leak :
    // -Execution normale : DONE
    // -Execution avec erreur : DONE

    // TODO : make a bytecode output
    // TODO : make a bytecode input

    while (1) {

        Settings_t settings;
        settings.flags = !FLAG_NO_INTERPRETOR | !FLAG_NO_TYPECHECK | FLAG_PRINT_PARSER_OUTPUT | FLAG_PRINT_STACK | !FLAG_PRINT_MEMORY;

        char *code = (char *) c_malloc(sizeof(char) * 1001);
        c_fskprint("Keashell >>> ");
        c_input(code, 1000, c_blue);

        c_fskprint("\n");

        LexedProgram_t program = run_lexer(code, settings);
        ParsedProgram_t parsed_program = run_parser(program, settings, 0);

        int error_code = run_interpretor(parsed_program, settings);
        if (error_code != NO_ERROR) {
            c_fskprint("Error: interpretor failed with error code %d\n", error_code);
        }

        c_free(code);

        if (settings.flags & FLAG_PRINT_MEMORY) {
            c_mem_print();
        }

    }

    while (1);
    return 0;
}
