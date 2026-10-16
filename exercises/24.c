#include "exercises.h"
#include "lib.h"

/* Compare count bytes as unsigned bytes, returning a sign. */
int my_memcmp(const void *a, const void *b, size_t count)
{
    (void)a; (void)b; (void)count;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("comparison = %d\n", my_memcmp("abc", "abd", 3));
    return 0;
}
#endif
