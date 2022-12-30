#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <i_time.h>

char **text_buffer;
int lines_number;
int current_line;
int current_col;
int *line_sizes;

int main(int argc, char **argv) {

    text_buffer = calloc(100, sizeof(char *)); // hardcoded for now
    for (int i = 0; i < 100; i++) {
        text_buffer[i] = calloc(100, sizeof(char)); // hardcoded for now
    }
    current_line = 0;
    current_col = 0;
    line_sizes = calloc(100, sizeof(int)); // hardcoded for now
    
    for (int i = 0; i < 100; i++) {
        line_sizes[i] = 0;
    }
    lines_number = 100;

    if (argc == 2) {
        // we open a new file that will be saved later
    }
    else {
        // we open the file that will be edited
        char *filename = calloc(strlen(argv[1]) + 1 + strlen(argv[2]) + 1, sizeof(char));
        strcpy(filename, argv[1]);
        strcat(filename, "/");
        strcat(filename, argv[2]);
        FILE *file = fopen(filename, "r+");
        if (file == NULL) {
            printf("File %s does not exist\n", filename);
            return 1;
        }
        free(filename);

        char *file_content = calloc(file->buffer_size + 1, sizeof(char));
        fread(file_content, file->buffer_size, file->buffer_size, file);
        fclose(file);
        file_content[file->buffer_size] = '\0';
        int index = 0;
        while (file_content[index] != '\0') {
            printf("%c", file_content[index]);
        }
    }
    
    int old_cursor = c_get_cursor_offset();
    int buffer_actual_size = 0;
    int history_index = 0;
    int buffer_index = 0;
    int key_ticks = 0;
    int sc, last_sc;
    int shift = 0;
    int new_pos;


    #define FIRST_L 40
    #define BONDE_L 4

    int row = c_gt_get_max_rows();
    int col = c_gt_get_max_cols();

    while (1) {
        ms_sleep(2);
        for (int i = 0; i < lines_number; i++) {
            printf("%s", text_buffer[i]);
            if (i != lines_number - 1) printf("\n");
        }

        last_sc = sc;
        sc = c_kb_get_scfh();
        
        if (sc != last_sc) key_ticks = 0;
        else key_ticks++;

        if ((key_ticks > 2 && key_ticks < FIRST_L) || key_ticks % BONDE_L) continue;

        if (sc == LSHIFT || sc == RSHIFT) {
            shift = 1;
            continue;
        }

        else if (sc == LSHIFT + 128 || sc == RSHIFT + 128) {
            shift = 0;
            continue;
        }
        
        else if (sc == LEFT) {
            // do nothing
        }

        else if (sc == RIGHT) {
            // do nothing
        }

        else if (sc == OLDER) {
            // do nothing
        }

        else if (sc == NEWER) {
            // do nothing
        }

        else if (sc == BACKSPACE) {
            // we remove the last character
            if (current_col > 0) {
                current_col--;
                text_buffer[current_line][current_col] = '\0';
            } else if (current_line > 0) {
                current_line--;
                current_col = line_sizes[current_line];
                text_buffer[current_line][current_col] = '\0';
            }
        }

        else if (sc == DEL) {
            // do nothing
        }

        else if (sc == KB_TAB) {
            for (int i = 0; i < 4; i++) {
                // we add a tab
                text_buffer[current_line][current_col] = ' ';
                current_col++;
            }
        }

        else if (sc == ENTER) {
            // we add a new line
            current_line++;
            current_col = 0;
        }

        else if (sc <= SC_MAX) {
            if (c_kb_scancode_to_char(sc, shift) == '?') continue;
            // we add the character to the buffer
            text_buffer[current_line][current_col] = c_kb_scancode_to_char(sc, shift);
            current_col++;
        }

    }
}

