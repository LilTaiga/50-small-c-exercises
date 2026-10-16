#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Append an integer, growing owned storage when necessary. */
int vector_push(IntVector *vector, int value)
{
    (void)vector; (void)value;
    /* TODO: implement. */
    return -1;
}

#ifndef EXERCISE_TEST
int main(void)
{
    IntVector vector = {0};
    int status = vector_push(&vector, 42);
    printf("status=%d, length=%zu\n", status, vector.length);
    free(vector.data);
    return 0;
}
#endif
