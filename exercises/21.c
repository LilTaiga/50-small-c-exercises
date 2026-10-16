#include "exercises.h"
#include "lib.h"

/* Fill count bytes with value converted to unsigned char. */
void *my_memset(void *destination, int value, size_t count)
{
    (void)destination; (void)value; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[] = "hello";
    my_memset(text, '*', 3);
    printf("%s\n", text);
    return 0;
}
#endif
