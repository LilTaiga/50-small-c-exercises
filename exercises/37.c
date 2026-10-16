#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate an independent copy of count integers. */
int *array_clone(const int *values, size_t count)
{
    (void)values; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const int values[] = {3, 1, 4};
    int *copy = array_clone(values, 3);
    if (copy) printf("first value = %d\n", copy[0]);
    else puts("No copy returned.");
    free(copy);
    return 0;
}
#endif
