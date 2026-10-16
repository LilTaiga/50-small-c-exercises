#include "exercises.h"
#include "lib.h"

/* Find the first matching unsigned byte within count bytes. */
void *my_memchr(const void *memory, int value, size_t count)
{
    (void)memory; (void)value; (void)count;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const char text[] = "hello";
    printf("found = %s\n", my_memchr(text, 'l', 5) ? "yes" : "no");
    return 0;
}
#endif
