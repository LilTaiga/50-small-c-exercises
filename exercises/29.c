#include "exercises.h"
#include "lib.h"

/* Find the first string character belonging to accepted. */
char *my_strpbrk(const char *s, const char *accepted)
{
    (void)s; (void)accepted;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const char *found = my_strpbrk("hello,world", ",;");
    printf("suffix = %s\n", found ? found : "(not found)");
    return 0;
}
#endif
