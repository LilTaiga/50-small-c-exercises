#include "exercises.h"
#include "lib.h"

/* Keep value inside the inclusive range [minimum, maximum]. */
int clamp_int(int value, int minimum, int maximum)
{
    (void)value; (void)minimum; (void)maximum;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("clamp_int(15, 0, 10) = %d\n", clamp_int(15, 0, 10));
    return 0;
}
#endif
