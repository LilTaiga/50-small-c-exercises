#include "exercises.h"
#include "lib.h"

/* Return the last matching character's address, or NULL. */
char *my_strrchr(const char *s, int c)
{
    (void)s; (void)c;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const char *found = my_strrchr("banana", 'n');
    printf("suffix = %s\n", found ? found : "(not found)");
    return 0;
}
#endif
