#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Split into separately allocated fields; preserve empty fields. */
char **string_split(const char *s, char delimiter, size_t *count)
{
    (void)s; (void)delimiter; (void)count;
    /* TODO: implement. */
    return NULL;
}

/* Release every field and the pointer array. */
void string_split_free(char **strings, size_t count)
{
    (void)strings; (void)count;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void)
{
    size_t count = 0;
    char **fields = string_split("red,green,blue", ',', &count);
    printf("fields = %zu\n", count);
    string_split_free(fields, count);
    return 0;
}
#endif
