#include "exercises.h"
#include "lib.h"

/* Sort count integers in ascending order using insertion sort. */
void array_sort(int *values, size_t count)
{
    (void)values; (void)count;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void)
{
    int values[] = {3, 1, 2};
    array_sort(values, 3);
    printf("[%d, %d, %d]\n", values[0], values[1], values[2]);
    return 0;
}
#endif
