#include "exercises.h"
#include "lib.h"

/* Write count bytes, copying source then padding with null bytes. */
char *my_strncpy(char *destination, const char *source, size_t count)
{
    (void)destination; (void)source; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[8] = {0};
    my_strncpy(text, "hello", 7);
    printf("%s\n", text);
    return 0;
}
#endif
