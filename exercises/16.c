#include "exercises.h"
#include "lib.h"

/* Append source to destination; return destination. */
char *my_strcat(char *destination, const char *source)
{
    (void)destination; (void)source;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[32] = "hello";
    my_strcat(text, " C");
    printf("%s\n", text);
    return 0;
}
#endif
