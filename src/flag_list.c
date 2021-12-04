#include "../inc/header.h"

void define_flags(arguments *uls) {
    for (int i = 0; i < 16; i++) {
        uls->flags[i] = false;
    }
    if (uls->argc < 2) {
        uls->flags[0] = true;
	}
    else {
        for (int i = 1; i < uls->argc; i++) {
            
            if (uls->argv[i][0] == '-') { //если у нас флаг
                if (!uls->argv[i][1]) {
                    mx_printerr("uls: ");
                    mx_printerr(uls->argv[i]);
                    mx_printerr(": No such file or directory");
                    exit(-1);
                }
                else {
                    for (int j = 1; j < mx_strlen(uls->argv[i]); j++) {
                        
                        if (uls->argv[i][j] == 'l') { // +
                            uls->flags[1] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'R') { // +
                            uls->flags[2] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'a') { // +
                            uls->flags[3] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'A') { // +
                            uls->flags[4] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'h') {
                            uls->flags[5] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == '@') { // +
                            uls->flags[6] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'e') { // ??
                            uls->flags[7] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'T') { // ??
                            uls->flags[8] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == '1') { // +
                            if (uls->flags[1] == true)
                                uls->flags[1] = false;
                            uls->flags[9] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'C') {
                            uls->flags[10] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'r') { // +
                            uls->flags[11] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 't') { // +-
                            uls->flags[12] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'u') { // +-
                            uls->flags[13] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'c') { // +-
                            uls->flags[14] = true;
                            continue;
                        }
                        if (uls->argv[i][j] == 'S') { // +-
                            uls->flags[15] = true;
                            continue;
                        }
                        mx_printerr("uls: illegal option -- ");
                        write(STDERR_FILENO, &uls->argv[i][j], 1);
                        mx_printerr("\nusage: uls [-1@ACRSTacehlrtu] [file ...]\n");
                        exit(-1);
                    }
                }
            }
            else {
                uls->directory = true;
            }
        }
    }
}

