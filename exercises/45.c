#include "exercises.h"
#include "lib.h"

/* Binary-search sorted fixed-size records using a comparator callback. */
void *my_bsearch(const void *key, const void *base, size_t count, size_t size,
                 int (*compare)(const void *, const void *)) {
    (void)key;
    (void)base;
    (void)count;
    (void)size;
    (void)compare;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void) { return 0; }
#endif
