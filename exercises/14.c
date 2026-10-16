#include "exercises.h"
#include "lib.h"

/* Return the first matching character's address, or NULL. */
char *my_strchr(const char *s, int c)
{
    (void)s; (void)c;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const char *found = my_strchr("banana", 'n');
    printf("suffix = %s\n", found ? found : "(not found)");
    return 0;
}
#endif
