#include "../inc/libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    int len = 0;
    int i = 0;
    unsigned long number = 0;
    unsigned long base = 1;

    if(hex != NULL) {
        while (hex[len]) {
            len++;
        }
    }
    else {
        return 0;
    }
    for (i = len--; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            number += (hex[i] - 48) * base;
            base *= 16;
        }
        else {
            if (hex[i] >= 'A' && hex[i] <= 'F') {
                number += (hex[i] - 55) * base;
                base *= 16;
            }
            else {
                if (hex[i] >= 'a' && hex[i] <= 'f') {
                    number += (hex[i] - 87) * base;
                    base *= 16;
                }
            }
        }
    }
    return number;
}

