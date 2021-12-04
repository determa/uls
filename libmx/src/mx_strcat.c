#include "../inc/libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
    int i =  mx_strlen(s1), b;
    for (b = 0; s2[b] != '\0'; b++) {
        s1[i + b] = s2[b];
    }
    s1[i + b] = '\0';
    return s1;
}

