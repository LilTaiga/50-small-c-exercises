#include "exercises.h"
#include "lib.h"

/* Copy count bytes between non-overlapping memory regions. */
void *my_memcpy(void *destination, const void *source, size_t count)
{
    (void)destination; (void)source; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[6] = {0};
    my_memcpy(text, "hello", 6);
    printf("%s\n", text);
    return 0;
}
#endif
