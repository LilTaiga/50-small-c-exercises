#include "exercises.h"
#include "lib.h"

/* Binary-search sorted fixed-size records using a comparator callback. */
void *my_bsearch(const void *key, const void *base, size_t count, size_t size,
                 int (*compare)(const void *, const void *))
{
    (void)key; (void)base; (void)count; (void)size; (void)compare;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
static int compare_ints(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
int main(void)
{
    int values[] = {1, 3, 5}, key = 3;
    int *found = my_bsearch(&key, values, 3, sizeof *values, compare_ints);
    printf("found = %s\n", found ? "yes" : "no");
    return 0;
}
#endif
