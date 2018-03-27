#include <stdio.h>

void main() {
    float total = 0;
    float in = 0;
    do {
        total+=in;
        scanf("%f", &in);
    } while (in != -1);
    printf("%f\n", total);
}
