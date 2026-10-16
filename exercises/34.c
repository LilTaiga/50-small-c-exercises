#include "exercises.h"
#include "lib.h"

/* Parse a whole unsigned number in a caller-selected base. */
ParseResult parse_uint_base(const char *s, unsigned base, unsigned *result)
{
    (void)s; (void)base; (void)result;
    /* TODO: implement. */
    return PARSE_INVALID;
}

#ifndef EXERCISE_TEST
int main(void)
{
    unsigned value = 0;
    ParseResult status = parse_uint_base("FF", 16, &value);
    printf("status=%d, value=%u\n", (int)status, value);
    return 0;
}
#endif
