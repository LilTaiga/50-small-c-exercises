#include "exercises.h"
#include "lib.h"

/* Parse a whole decimal string, reporting syntax and range errors. */
ParseResult parse_int(const char *s, int *result)
{
    (void)s; (void)result;
    /* TODO: implement. */
    return PARSE_INVALID;
}

#ifndef EXERCISE_TEST
int main(void)
{
    int value = 0;
    ParseResult status = parse_int("-42", &value);
    printf("status=%d, value=%d\n", (int)status, value);
    return 0;
}
#endif
