#include "exercises.h"
#include "lib.h"

/* Decode four little-endian bytes without alignment assumptions. */
uint32_t read_u32_le(const unsigned char bytes[4])
{
    (void)bytes;
    /* TODO: implement. */
    return 0;
}

#ifndef EXERCISE_TEST
int main(void)
{
    const unsigned char bytes[] = {0x78, 0x56, 0x34, 0x12};
    printf("value = 0x%08lx\n", (unsigned long)read_u32_le(bytes));
    return 0;
}
#endif
