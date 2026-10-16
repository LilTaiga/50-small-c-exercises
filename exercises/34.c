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
    return 0;
}
#endif
