#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate strings joined by a separator. */
char *string_join(const char *const *strings, size_t count, const char *separator)
{
    (void)strings; (void)count; (void)separator;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const char *words[] = {"red", "green", "blue"};
    char *joined = string_join(words, 3, ", ");
    printf("joined = %s\n", joined ? joined : "(no result)");
    free(joined);
    return 0;
}
#endif
