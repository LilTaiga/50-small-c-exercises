#include "exercises.h"
#include "lib.h"
#include <stdlib.h>

/* Read one owned line of arbitrary length; distinguish EOF from errors. */
int read_line(FILE *stream, char **line, size_t *length)
{
    (void)stream; (void)line; (void)length;
    /* TODO: implement. */
    return -1;
}

#ifndef EXERCISE_TEST
int main(void)
{
    FILE *stream = tmpfile();
    if (!stream) return 1;
    fputs("hello world\n", stream);
    rewind(stream);
    char *line = NULL;
    size_t length = 0;
    int status = read_line(stream, &line, &length);
    printf("status=%d, length=%zu, line=%s\n", status, length, line ? line : "(none)");
    free(line);
    fclose(stream);
    return 0;
}
#endif
