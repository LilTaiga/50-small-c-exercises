#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Unlink and free nodes selected by a callback; return removal count. */
size_t list_remove_if(IntNode **head, int (*predicate)(int))
{
    (void)head; (void)predicate;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
static int is_negative(int value) { return value < 0; }
int main(void)
{
    IntNode *head = NULL;
    /* Build a list here, using your completed helpers in lib.c. */
    printf("removed = %zu\n", list_remove_if(&head, is_negative));
    return 0;
}
#endif
