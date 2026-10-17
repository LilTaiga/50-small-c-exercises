#include "exercises.h"
#include "lib.h"

/* Recognize ASCII letters, without locale-dependent library functions. */
int my_isalpha(int c) {
    (void)c;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void) {
    printf("my_isalpha('G') = %d\n", my_isalpha('G'));
    return 0;
}
#endif
