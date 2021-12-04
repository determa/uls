#include "../inc/header.h"

files *find_node(files *list, int index) {
    if (index < 0) {
        return NULL;
    }
    files *t = list;
    for (int i = 0; t != NULL; i++) {
        if (i == index) {
            return t;
        }
        else {
            t = t->next;
        }
    }
    return NULL;
}

