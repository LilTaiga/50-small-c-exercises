#include "exercises.h"
#include "lib.h"

/* Tokenize a mutable string using caller-owned continuation state. */
char *next_token(char *s, const char *delimiters, char **state)
{
    (void)s; (void)delimiters; (void)state;
    /* TODO: implement. */
    return NULL;
}

#ifndef EXERCISE_TEST
int main(void)
{
    char text[] = "red,green,blue", *state = NULL;
    char *token = next_token(text, ",", &state);
    printf("first token = %s\n", token ? token : "(none)");
    return 0;
}
#endif
