#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate an independent copy of a string. */
char *my_strdup(const char *s)
{
    (void)s;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char *copy = my_strdup("hello");
    printf("copy = %s\n", copy ? copy : "(allocation failed)");
    free(copy);
    return 0;
}
#endif
