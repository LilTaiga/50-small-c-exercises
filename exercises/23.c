#include "exercises.h"
#include "lib.h"

/* Copy count bytes correctly even when the regions overlap. */
void *my_memmove(void *destination, const void *source, size_t count)
{
    (void)destination; (void)source; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[] = "abcdef";
    my_memmove(text + 1, text, 5);
    printf("%s\n", text);
    return 0;
}
#endif
