#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Append arbitrary bytes to a growable owned buffer. */
int buffer_append(ByteBuffer *buffer, const void *data, size_t count)
{
    (void)buffer; (void)data; (void)count;
    /* TODO: implement. */
    return -1;
}

#ifndef EXERCISE_TEST
int main(void)
{
    ByteBuffer buffer = {0};
    int status = buffer_append(&buffer, "hello", 5);
    printf("status=%d, bytes=%zu\n", status, buffer.length);
    free(buffer.data);
    return 0;
}
#endif
