#include "support.h"
#include <string.h>

#if EXERCISE == 11
void run_tests(void)
{
    CASE("\"hello\""); UINT(5, my_strlen("hello"));
    CASE("\"\""); UINT(0, my_strlen(""));
    CASE("\"a\\0b\\0\""); const char s[] = {'a', 0, 'b', 0}; UINT(1, my_strlen(s));
    CASE("\"a\\nb\""); UINT(3, my_strlen("a\nb"));
    char long_string[1025]; memset(long_string, 'x', 1024); long_string[1024] = 0;
    CASE("1024 copies of x, then terminator"); UINT(1024, my_strlen(long_string));
}
#elif EXERCISE == 12
void run_tests(void)
{
    char dst[8] = "XXXXXXX"; const char src[] = "cat";
    CASE("source=\"cat\", destination=\"XXXXXXX\"");
    POINTER(dst, sizeof dst, dst, my_strcpy(dst, src));
    BYTES("cat\0XXX", dst, 8); STRING("cat", src);
    CASE("source=\"\", destination still contains previous bytes");
    POINTER(dst, sizeof dst, dst, my_strcpy(dst, ""));
    BYTES("\0at\0XXX", dst, 8);
    char exact[2] = {'X', 'X'};
    CASE("source=\"a\", capacity=2"); my_strcpy(exact, "a"); BYTES("a", exact, 2);
}
#elif EXERCISE == 13 || EXERCISE == 17
void run_tests(void)
{
    const char *a[] = {"cat", "cat", "", "a", "abc", "abd", "\xFF", "\x7F"};
    const char *b[] = {"dog", "cat", "a", "", "abcd", "abc", "\x7F", "\xFF"};
    for (size_t i = 0; i < sizeof a / sizeof *a; ++i) {
#if EXERCISE == 13
        PAIR_CASE(a[i], b[i]);
        INT(sign_of(strcmp(a[i], b[i])), sign_of(my_strcmp(a[i], b[i])));
#else
        for (size_t count = 0; count <= 6; ++count) {
            BOUNDED_PAIR_CASE(a[i], b[i], count);
            INT(sign_of(strncmp(a[i], b[i], count)), sign_of(my_strncmp(a[i], b[i], count)));
        }
#endif
    }
}
#elif EXERCISE == 14 || EXERCISE == 15
void run_tests(void)
{
    char s[] = "banana";
    const int targets[] = {'b', 'a', 'n', 'x', '\0'};
    for (size_t i = 0; i < sizeof targets / sizeof *targets; ++i) {
        char label[80]; snprintf(label, sizeof label, "string=\"banana\", target=%d (0x%02X)", targets[i], targets[i]); CASE(label);
#if EXERCISE == 14
        POINTER(s, sizeof s, strchr(s, targets[i]), my_strchr(s, targets[i]));
#else
        POINTER(s, sizeof s, strrchr(s, targets[i]), my_strrchr(s, targets[i]));
#endif
    }
    char empty[] = ""; CASE("empty string, target=null byte then x");
#if EXERCISE == 14
    POINTER(empty, sizeof empty, empty, my_strchr(empty, 0));
    POINTER(empty, sizeof empty, NULL, my_strchr(empty, 'x'));
#else
    POINTER(empty, sizeof empty, empty, my_strrchr(empty, 0));
    POINTER(empty, sizeof empty, NULL, my_strrchr(empty, 'x'));
#endif
    STRING("banana", s);
}
#elif EXERCISE == 16 || EXERCISE == 19
void run_tests(void)
{
    const char *prefixes[] = {"hello", "", "a", "abc"};
    const char *suffixes[] = {" C", "xyz", "", "defgh"};
    for (size_t i = 0; i < 4; ++i) {
        for (size_t n = 0; n <= 6; ++n) {
            char actual[32], expected[32];
            memset(actual, 0x55, sizeof actual); strcpy(actual, prefixes[i]);
            memcpy(expected, actual, sizeof actual);
            char label[120]; snprintf(label, sizeof label, "destination=\"%s\", source=\"%s\", count=%zu (bounded version only)", prefixes[i], suffixes[i], n); CASE(label);
#if EXERCISE == 16
            strcat(expected, suffixes[i]);
            POINTER(actual, sizeof actual, actual, my_strcat(actual, suffixes[i]));
#else
            strncat(expected, suffixes[i], n);
            POINTER(actual, sizeof actual, actual, my_strncat(actual, suffixes[i], n));
#endif
            BYTES(expected, actual, sizeof actual);
        }
    }
}
#elif EXERCISE == 18
void run_tests(void)
{
    const char *sources[] = {"", "a", "hello"};
    for (size_t i = 0; i < 3; ++i) {
        for (size_t n = 0; n <= 7; ++n) {
            char actual[10], expected[10]; memset(actual, 0x55, 10); memcpy(expected, actual, 10);
            char label[80]; snprintf(label, sizeof label, "source=\"%s\", count=%zu, destination filled with 0x55", sources[i], n); CASE(label);
            strncpy(expected, sources[i], n);
            POINTER(actual, sizeof actual, actual, my_strncpy(actual, sources[i], n));
            BYTES(expected, actual, 10);
        }
    }
}
#elif EXERCISE == 20
void run_tests(void)
{
    const char *inputs[] = {"hello", "abcd", "", "a", "abba", "a\nb"};
    const char *outputs[] = {"olleh", "dcba", "", "a", "abba", "b\na"};
    for (size_t i = 0; i < 6; ++i) {
        char actual[16], expected[16]; memset(actual, 0x55, sizeof actual); strcpy(actual, inputs[i]);
        memcpy(expected, actual, sizeof actual); strcpy(expected, outputs[i]);
        STRING_CASE(inputs[i]);
        reverse_string(actual); BYTES(expected, actual, sizeof actual);
    }
}
#endif
