#include "../inc/header.h"

void sort_argv(char *argv[], int argc) {
    char *temp = NULL;
    for (int i = 1; i < argc - 1; i++) {
        for (int j = 1; j <= argc - i - 1; j++) {
            if (mx_strcmp(argv[j], argv[j + 1]) > 0 && argv[j][0] != '-' && argv[j + 1][0] != '-') {
                temp = argv[j + 1];
                argv[j + 1] = argv[j];
                argv[j] = temp;
            }
        }
    }
    for (int i = 1; i < argc - 1; i++) {
        for (int j = 1; j <= argc - i - 1; j++) {
            if (is_dir(argv[j]) && !is_dir(argv[j + 1]) && argv[j][0] != '-' && argv[j + 1][0] != '-') {
                temp = argv[j + 1];
                argv[j + 1] = argv[j];
                argv[j] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    arguments *uls = (arguments *)malloc(sizeof(arguments));
    uls->directory = false;
    uls->argc = argc;
    uls->argv = (char **)malloc(sizeof(char *) * argc);
    uls->winsize = size.ws_col;
    bool all = false;
    for (int i = 0; i < argc; i++) {
        uls->argv[i] = argv[i];
    }
    define_flags(uls);
    if (uls->directory == false) {
        fill_list(uls, ".", NULL, all);
    }
    else {
        sort_argv(uls->argv, argc);
        argv_files(uls);
        uls->directory = false;
        for (int i = 1; i < argc; i++) {
            if (uls->argv[i][0] != '-') {
                if (is_dir(uls->argv[i])) {
                    if (uls->count_direct > 1 || uls->count_files > 0) {
                        if ((i > 1 && !uls->flags[1])
                            || (i > 2 && uls->flags[1]))
                            mx_printstr("\n");
                        mx_printstr(uls->argv[i]);
                        mx_printstr(":\n");
                    }
                    fill_list(uls, uls->argv[i], NULL, all);
                }
            }
        }
    }
    free(uls);
    return 0;
}

