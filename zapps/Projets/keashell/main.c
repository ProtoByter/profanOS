#include <syscall.h>

// CHECK EVERYTHING

typedef struct Element_t {
    int data_type;
    int data_int;
    char *data_str;
} Element_t;

typedef struct Instruction_t {
    char *name;
    int data_type;
    void **data_Element;
    void **data_Instruction;
} Instruction_t;

typedef struct ElementPile_t {
    Element_t **data;
    int size_max;
    int size;
} ElementPile_t;

typedef struct Instructionpile_t {
    Instruction_t **data;
    int size_max;
    int size;
} Instructionpile_t;

void compile(char *code, Instructionpile_t *liste_instructions);
Instructionpile_t Instructionpile_init(int size_max);

int main(int argc, char **argv) {
    
    char code[] = "3 4 + .";
    c_fskprint("Lancement du code : %s\n", code);
    Instructionpile_t liste_instructions = Instructionpile_init(100);
    compile(code, &liste_instructions);
    c_fskprint("Fin du programme\n");
    while (1);
    return 0;
}

void add_ElementPile(ElementPile_t pile, Element_t element) {
    if (pile.size == pile.size_max) {
        c_fskprint("STACK OVERFLOWWWKJHKJSDHFLSDHFJKSDFKJHS");
        return;
    }
    pile.data[pile.size] = &element;
    pile.size++;
}
Element_t remove_ElementPile(ElementPile_t pile) {
    if (pile.size == 0) {
        c_fskprint("STACK UNDERFLOWWWKJHKJSDHFLSDHFJKSDFKJHS");
    }
    pile.size--;
    return *pile.data[pile.size];
}
void add_InstructionPile(Instructionpile_t pile, Instruction_t instruction) {
    if (pile.size == pile.size_max) {
        c_fskprint("STACK OVERFLOWWWKJHKJSDHFLSDHFJKSDFKJHS");
        return;
    }
    pile.data[pile.size] = &instruction;
    pile.size++;
}
Instruction_t remove_InstructionPile(Instructionpile_t pile) {
    if (pile.size == 0) {
        c_fskprint("STACK UNDERFLOWWWKJHKJSDHFLSDHFJKSDFKJHS");
    }
    pile.size--;
    return *pile.data[pile.size];
}
Instructionpile_t Instructionpile_init(int size_max) {
    Instructionpile_t pile;
    pile.data = c_malloc(size_max * sizeof(Instruction_t));
    pile.size_max = size_max;
    pile.size = 0;
    return pile;
}
ElementPile_t ElementPile_init(int size_max) {
    ElementPile_t pile;
    pile.data = c_malloc(sizeof(Element_t *) * size_max);
    pile.size_max = size_max;
    pile.size = 0;
    return pile;
}

void compile(char *code, Instructionpile_t *liste_instructions) {
    c_str_append(code, ' ');
    int code_length = c_str_len(code);
    char *buffer = c_malloc(code_length);
    int i = 0;
    while (i < code_length) {
        // TODO
        i++;
    }
}
