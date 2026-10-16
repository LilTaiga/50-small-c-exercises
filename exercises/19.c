#include "exercises.h"
#include "lib.h"

/* Append at most count source bytes, then a terminator. */
char *my_strncat(char *destination, const char *source, size_t count)
{
    (void)destination; (void)source; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[32] = "hello";
    my_strncat(text, " C world", 2);
    printf("%s\n", text);
    return 0;
}
#endif
