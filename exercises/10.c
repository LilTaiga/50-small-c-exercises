#include "exercises.h"
#include "lib.h"

/* Sum count integers; all partial sums are representable as int. */
int array_sum(const int *values, size_t count) {
    (void)values;
    (void)count;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void) {
    const int values[] = {3, -1, 5};
    printf("sum = %d\n", array_sum(values, 3));
    return 0;
}
#endif
