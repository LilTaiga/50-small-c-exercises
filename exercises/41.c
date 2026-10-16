#include "exercises.h"
#include "lib.h"

/* Count one bits in an unsigned int. */
unsigned bit_count(unsigned value)
{
    (void)value;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    printf("bits in 0xA5 = %u\n", bit_count(0xA5u));
    return 0;
}
#endif
