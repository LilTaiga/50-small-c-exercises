#include "exercises.h"
#include "lib.h"

/* Swap the integers addressed by a and b. */
void swap_int(int *a, int *b) {
    (void)a;
    (void)b;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void) {
    int a = 3, b = 8;
    swap_int(&a, &b);
    printf("a=%d, b=%d\n", a, b);
    return 0;
}
#endif
