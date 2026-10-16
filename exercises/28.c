#include "exercises.h"
#include "lib.h"

/* Count leading bytes that do not belong to the rejected set. */
size_t my_strcspn(const char *s, const char *rejected)
{
    (void)s; (void)rejected;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("span = %zu\n", my_strcspn("hello,world", ",;"));
    return 0;
}
#endif
