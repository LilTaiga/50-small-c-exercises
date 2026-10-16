#include "exercises.h"
#include "lib.h"

/* Encode a 32-bit unsigned integer in four little-endian bytes. */
void write_u32_le(unsigned char bytes[4], uint32_t value)
{
    (void)bytes; (void)value;
    /* TODO: implement. */
}

#ifndef EXERCISE_TEST
int main(void)
{
    unsigned char bytes[4] = {0};
    write_u32_le(bytes, UINT32_C(0x12345678));
    for (size_t i = 0; i < 4; ++i) printf("%02X%s", (unsigned)bytes[i], i == 3 ? "\n" : " ");
    return 0;
}
#endif
