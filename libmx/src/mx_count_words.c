#include "../inc/libmx.h"

int mx_count_words(const char *str, char c) {
    int word = 0, i = 0;
    if (str[0] != c && str[0] != '\0') {
        word++;
        i++;
    }
    while (str[i] != '\0') {
        if (str[i] == c) {
            while (str[i] != '\0' && str[i] == c) {
                i++;
            }
            if(str[i] != c && str[i] != '\0') {
                word++;
            }
        }
        i++;
    }
    return word;
}

