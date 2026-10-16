#include "exercises.h"
#include "lib.h"

/* Count bytes before the terminating null byte. */
size_t my_strlen(const char *s)
{
    (void)s;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("length = %zu\n", my_strlen("hello"));
    return 0;
}
#endif
