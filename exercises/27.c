#include "exercises.h"
#include "lib.h"

/* Count leading bytes that all belong to the accepted set. */
size_t my_strspn(const char *s, const char *accepted)
{
    (void)s; (void)accepted;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("span = %zu\n", my_strspn("123abc", "0123456789"));
    return 0;
}
#endif
