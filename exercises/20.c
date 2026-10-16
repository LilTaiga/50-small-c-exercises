#include "exercises.h"
#include "lib.h"

/* Reverse a writable string's bytes in place, preserving its terminator. */
void reverse_string(char *s)
{
    (void)s;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[] = "hello";
    reverse_string(text);
    printf("%s\n", text);
    return 0;
}
#endif
