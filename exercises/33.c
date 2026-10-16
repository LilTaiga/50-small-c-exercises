#include "exercises.h"
#include "lib.h"

/* Format an int as decimal into a caller-owned buffer. */
int int_to_string(int value, char *destination, size_t capacity)
{
    (void)value; (void)destination; (void)capacity;
    /* TODO: implement. */
    return -1;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[64] = "";
    int status = int_to_string(-42, text, sizeof text);
    printf("status=%d, text=\"%s\"\n", status, text);
    return 0;
}
#endif
