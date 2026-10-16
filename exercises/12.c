#include "exercises.h"
#include "lib.h"

/* Copy a string and its terminator; return destination. */
char *my_strcpy(char *destination, const char *source)
{
    (void)destination; (void)source;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[32] = "";
    my_strcpy(text, "hello");
    printf("text = \"%s\"\n", text);
    return 0;
}
#endif
