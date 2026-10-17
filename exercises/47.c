#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Append arbitrary bytes to a growable owned buffer. */
int buffer_append(ByteBuffer *buffer, const void *data, size_t count) {
    (void)buffer;
    (void)data;
    (void)count;
    /* TODO: implement. */
    return -1;
}

#ifndef EXERCISE_TEST
int main(void) { return 0; }
#endif
