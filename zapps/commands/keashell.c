#include <syscall.h>

typedef struct Element {
    int data_type;
    int data_int;
    char* data_string;
} Element;

typedef struct Instruction {
    char *name;
    Element element;
} Instruction;

typedef struct InstPile {
    Instruction *inst;
    int size;
    int top;
} InstPile;

typedef struct Pile {
    Element* elements;
    int size;
    int top;
} Pile;

typedef struct Function {
    int nb_args;
    int nb_return;
    void * function;
} Function;

void add_instruction(char* inst, InstPile *liste_instructions);
void add_buffer(char* buffer1, InstPile *liste_instructions);
void compileall(char* code, InstPile *liste_instructions);
void run(InstPile *liste_instructions);
InstPile Instpile_init(int size);

int main(int argc, char **argv) {
    char code[] = "1,2,3,4>>>>+,+>>+>print";
    int code_size = c_str_len(code);
    InstPile liste_instructions = Instpile_init(code_size);
    compileall(code, &liste_instructions);
    run(&liste_instructions);
    for (int i = 0; i < liste_instructions.top+1; i++) {
        c_free(liste_instructions.inst[i].element.data_string);
    }
    c_free(liste_instructions.inst);
    return 0;
}

InstPile Instpile_init(int size) {
    InstPile pile;
    pile.inst = c_malloc(sizeof(Instruction) * size);
    pile.size = size;
    pile.top = -1;
    return pile;
}

void Instpile_push(InstPile *pile, Instruction inst) {
    if (pile->top == pile->size - 1) {
        c_fskprint("Stack overflow\n");
    }
    pile->inst[++pile->top] = inst;
}

Instruction Instpile_pop(InstPile *pile) {
    if (pile->top == -1) {
        c_fskprint("Stack underflow\n");
    }
    return pile->inst[pile->top--];
}

Pile pile_init(int size) {
    Pile pile;
    pile.elements = c_malloc(sizeof(Element) * size);
    pile.size = size;
    pile.top = -1;
    return pile;
}

void pile_push(Pile *pile, Element element) {
    if (pile->top == pile->size - 1) {
        c_fskprint("Stack overflow\n");
    }
    pile->elements[++pile->top] = element;
}

Element pile_pop(Pile *pile) {
    if (pile->top == -1) {
        c_fskprint("Stack underflow\n");
    }
    return pile->elements[pile->top--];
}

void add2(Pile *pile, Pile *liste_args) {
    c_fskprint("add2\n");
    Element arg1 = pile_pop(liste_args);
    Element arg2 = pile_pop(liste_args);
    if (arg1.data_type == 0 && arg2.data_type == 0) {
        Element res;
        res.data_type = 0;
        res.data_int = arg1.data_int + arg2.data_int;
        pile_push(pile, res);
        return;
    }
}

void afficher(Pile *pile, Pile *liste_args) {
    c_fskprint("afficher\n");
    Element arg1 = pile_pop(liste_args);
    if (arg1.data_type == 0) {
        c_fskprint("%d", arg1.data_int);
        return;
    }
}

#define NB_BUILDINS 2
#define NB_ALIAS_MAX 3
#define NB_MAX_SIZE 10

char buildins_names[NB_BUILDINS][NB_ALIAS_MAX][NB_MAX_SIZE] = {
    {"add", "+", ""},
    {"print", "afficher", ""}
};

void copy_fonction_struc(Function *dest, Function *src) {
    dest->nb_args = src->nb_args;
    dest->nb_return = src->nb_return;
    dest->function = src->function;
}

void add_instruction(char* inst, InstPile *liste_instructions) {
    int is_num = 1;
    char* liste_num = "0123456789";
    for (int i = 0; i < c_str_len(inst); i++) {
        int temp = 0;
        for (int j=0; j < c_str_len(liste_num); j++) {
            if (inst[i] == liste_num[j]) {
                temp = 1;
            }
        }
        if (temp == 0) {
            is_num = 0;
        }
    }
    if (is_num == 1) {
        int nb = 0;
        nb = c_ascii_to_int(inst);
        Instpile_push(liste_instructions, (Instruction) {
            .name = "addnb",
            .element = (Element) {
                .data_type = 0,
                .data_int = nb,
                .data_string = c_malloc(1)
            }
        });
    } else {
        Instpile_push(liste_instructions, (Instruction) {
            .name = "cmd",
            .element = (Element) {
                .data_type = 1,
                .data_int = 0,
                .data_string = inst
            }
        });
    }
}

void add_buffer(char* buffer1, InstPile *liste_instructions) {
    int index = 0;
    char* buffer2 = c_malloc(sizeof(char) * 100);
    for (int i=0; i<100; i++) {
        buffer2[i] = '\0';
    }
    while (index < c_str_len(buffer1)) {
        if (buffer1[index] == ',') {
            if (c_str_len(buffer2) != 0) {
                char* buffer3 = c_malloc(sizeof(char) * 100);
                c_str_cpy(buffer3, buffer2);
                add_instruction(buffer3, liste_instructions);
                for (int i = 0; i < c_str_len(buffer2); i++) {
                    buffer2[i] = '\0';
                }
            } 
        } else {
            buffer2[c_str_len(buffer2)] = buffer1[index];
            buffer2[c_str_len(buffer2)] = '\0';
        }
        index++;
    }
    char* buffer3 = c_malloc(sizeof(char) * 100);
    c_str_cpy(buffer3, buffer2);
    add_instruction(buffer3, liste_instructions);
}

void compileall(char* code, InstPile *liste_instructions) {
    int index = 0;
    char *buffer = c_malloc(sizeof(char) * 100);
    for (int i=0; i<100; i++) {
        buffer[i] = '\0';
    }
    int nb_fleches = 0;
    while (index < c_str_len(code)) {
        if (code[index] == '>') {
            if (c_str_len(buffer) > 0) {
                char* buffer2 = c_malloc(sizeof(char) * 100);
                for (int i = 0; i < 100; i++) {
                    buffer2[i] = '\0';
                }
                c_str_cpy(buffer2, buffer);
                add_buffer(buffer2, liste_instructions);
                for (int i = 0; i < 100; i++) {
                    buffer[i] = '\0';
                }
            }
            nb_fleches++;
            index++;
            continue;
        }
        if (nb_fleches != 0) {
            Instpile_push(liste_instructions, (Instruction) {
                .name = "fleche",
                .element = (Element) {
                    .data_type = 0,
                    .data_int = nb_fleches,
                    .data_string = c_malloc(1)
                }
            });
            nb_fleches = 0;
        }
        buffer[c_str_len(buffer)] = code[index];
        buffer[c_str_len(buffer) + 1] = '\0';
        index++;
    }
    char* buffer2 = c_malloc(sizeof(char) * 100);
    for (int i = 0; i < 100; i++) {
        buffer2[i] = '\0';
    }
    c_str_cpy(buffer2, buffer);
    add_buffer(buffer2, liste_instructions);
}

void run(InstPile *liste_instructions) {
    Function buildins_functions[NB_BUILDINS] = {
        (Function){2, 1, add2},
        (Function){1, 0, afficher}
    };

    Pile pile = pile_init(100);
    Pile work_pile = pile_init(100);

    for (int i = 0; i < liste_instructions->top+1; i++) {
        // if (liste_instructions->inst[i].element.data_type == 0) {
        //     c_fskprint("inst[%d] = Instruction(%s, Element(%d))\n", i, liste_instructions->inst[i].name, liste_instructions->inst[i].element.data_int);
        // } else {
        //     c_fskprint("inst[%d] = Instruction(%s, Element(\"%s\"))\n", i, liste_instructions->inst[i].name, liste_instructions->inst[i].element.data_string);
        // }
        
        Instruction inst = liste_instructions->inst[i];

        if (!c_str_cmp(inst.name, "addnb")) {
            pile_push(&pile, inst.element);
        }

        else if (!c_str_cmp(inst.name, "fleche")) {
            for (int j=0; j<work_pile.top+1; j++) {
                pile_push(&pile, work_pile.elements[j]);
            }
            for (int j=0; j<inst.element.data_int; j++) {
                pile_push(&work_pile, pile_pop(&pile));
            }
        }
        
        else if (!c_str_cmp(inst.name, "cmd")) {
            for (int liste_id = 0; liste_id < NB_BUILDINS; liste_id++) {
                for (int alias_id = 0; alias_id < NB_ALIAS_MAX; alias_id++) {
                    if (!c_str_cmp(buildins_names[liste_id][alias_id], inst.element.data_string)) {
                        Function func = buildins_functions[liste_id];
                        c_fskprint("func = %s\n", buildins_names[liste_id][alias_id]);
                        ((void (*)(Pile*, Pile*)) func.function)(&pile, &work_pile);
                    }
                }
            }
            c_ms_sleep(250);
        }
        c_free(inst.element.data_string);
    }
    c_free(pile.elements);
    c_free(work_pile.elements);
}
