#include "exercises.h"
#include "lib.h"

/* Compare strings using unsigned-byte ordering; return a sign. */
int my_strcmp(const char *a, const char *b)
{
    (void)a; (void)b;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("comparison = %d\n", my_strcmp("cat", "dog"));
    return 0;
}
#endif
