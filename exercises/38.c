#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate a terminated copy of a specified string slice. */
char *string_slice(const char *s, size_t start, size_t length)
{
    (void)s; (void)start; (void)length;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char *slice = string_slice("hello world", 6, 5);
    printf("slice = %s\n", slice ? slice : "(no slice)");
    free(slice);
    return 0;
}
#endif
