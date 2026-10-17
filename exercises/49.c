#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Unlink and free nodes selected by a callback; return removal count. */
size_t list_remove_if(IntNode **head, int (*predicate)(int)) {
    (void)head;
    (void)predicate;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void) { return 0; }
#endif
