#include "../inc/libmx.h"

double mx_pow(double n, unsigned int pow) {
    double n2 = n;
    if (pow == 0) {
        return 1;
    }
    for (unsigned int i = 1; i < pow; i++) {
        n *= n2;
    }
    return n;
}

