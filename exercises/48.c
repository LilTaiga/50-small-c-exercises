#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Allocate and append one node to a singly linked list. */
int list_push_back(IntNode **head, int value)
{
    (void)head; (void)value;
    /* TODO: implement. */
    return -1;
}

/* Free all nodes without accessing them after release. */
void list_free(IntNode *head)
{
    (void)head;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void)
{
    IntNode *head = NULL;
    int status = list_push_back(&head, 42);
    printf("status=%d, head=%s\n", status, head ? "present" : "NULL");
    list_free(head);
    return 0;
}
#endif
