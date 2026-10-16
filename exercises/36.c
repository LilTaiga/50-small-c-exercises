#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate count * size zero bytes, checking multiplication first. */
void *my_calloc(size_t count, size_t size)
{
    (void)count; (void)size;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    unsigned char *bytes = my_calloc(4, 1);
    if (bytes) printf("first byte = %u\n", (unsigned)bytes[0]);
    else puts("No allocation returned.");
    free(bytes);
    return 0;
}
#endif
