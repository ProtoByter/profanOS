#include <kernel/snowflake.h>
#include <kernel/ramdisk.h>
#include <gui/gnrtx.h>
#include <gui/vesa.h>
#include <minilib.h>
#include <system.h>
#include <stdio.h>
#include <string.h>
#include <i_iolib.h>

#define BFR_SIZE 65

int shell_command(char command[]);

void start_kshell() {
    sys_warning("You are now in the kernel-level shell");
    kprint("\n");
    char char_buffer[BFR_SIZE];
    while (1) {
        rainbow_print("kernel-shell> ");
        input(char_buffer, BFR_SIZE, 9);
        kprint("\n");
        if (shell_command(char_buffer)) break;
        char_buffer[0] = '\0';
    }
    clear_screen();
}


void shell_so(char suffix[]) {
    char path[100] = "/bin/";
    str_cat(path, suffix);
    str_cat(path, ".bin");
    kprintf("path: %s\n", path);
    run_ifexist(path, 0, (char **)0);    
}

void shell_help() {
    char *help[] = {
        "ADDR   - show main address",
        "ALLOC  - allocate *0x1000",
        "EXIT   - quit the kshell",
        "GO     - go file as binary",
        "HELP   - show this help",
        "REBOOT - reboot the system",
        "SO     - run file in /bin",
    };

    for (int i = 0; i < ARYLEN(help); i++)
        kprintf("%s\n", help[i]);
}

void shell_addr() {
    printf("vesa fb: %x\n", vesa_get_framebuffer());
    printf("max add: %x (%fMo)\n", mem_get_info(0, 0), mem_get_info(0, 0) / 1024.0 / 1024.0);
    printf("ramdisk: %x (%fMo)\n", ramdisk_get_address(), ramdisk_get_size() / 2048.0);
    printf("mm base: %x\n", MEM_BASE_ADDR);
    printf("watfunc: %x\n", WATFUNC_ADDR);
}

int shell_command(char command[]) {
    char prefix[BFR_SIZE], suffix[BFR_SIZE];
    int part = 0;
    unsigned int i = 0;

    for (i = 0; i < strlen(command); i++) {
        if (command[i] == ' ') {
            prefix[i] = '\0';
            part = 1;
        }
        else if (part == 0) prefix[i] = command[i];
        else if (part == 1) suffix[i - strlen(prefix) - 1] = command[i];
    }
    if (part == 0) prefix[i] = '\0';
    else suffix[i - strlen(prefix) - 1] = '\0';

    if      (strcmp(prefix, "addr") == 0) shell_addr();
    else if (strcmp(prefix, "alloc") == 0) malloc(str2int(suffix) * 0x1000);
    else if (strcmp(prefix, "exit") == 0) return 1;
    else if (strcmp(prefix, "go") == 0) run_ifexist(suffix, 0, (char **)0);
    else if (strcmp(prefix, "help") == 0) shell_help();
    else if (strcmp(prefix, "reboot") == 0) sys_reboot();
    else if (strcmp(prefix, "so") == 0) shell_so(suffix);
    else if (prefix[0] != '\0') kprintf("not found: %s\n", prefix);

    return 0;
}
