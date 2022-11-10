#include <syscall.h>
#include "src/lexer.h"
#include "src/parser.h" 
#include "src/interpretor.h"

int main(int argc, char **argv) {
    // current memory leak :
    // -Execution normale : DONE
    // -Execution avec erreur : DONE

    char code[] = "3 4 + . \"test\" \"coucou\" + .";
    c_fskprint("Lancement du code : %s\n", code);

    LexedProgram_t program = run_lexer(code);
    ParsedProgram_t parsed_program = run_parser(program);
    int error_code = run_interpretor(parsed_program);
    if (error_code != NO_ERROR) {
        c_fskprint("Erreur %d\n", error_code);
    }
    
    c_mem_print();

    while (1);
    return 0;
}
