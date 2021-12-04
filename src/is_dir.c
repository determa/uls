#include "../inc/header.h"

bool is_dir(char *dir) {
    DIR *dh;
    dh = opendir(dir);
    if (!dh) {
        return false;
    } else {
        return true;
    }
}

