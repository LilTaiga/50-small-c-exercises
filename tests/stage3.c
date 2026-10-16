#include "support.h"
#include <string.h>

#if EXERCISE == 21
void run_tests(void)
{
    const int values[] = {0, '*', -1, 0x123};
    for (size_t v = 0; v < 4; ++v) for (size_t n = 0; n <= 8; ++n) {
        unsigned char actual[10], expected[10]; memset(actual, 0x55, 10); memcpy(expected, actual, 10);
        char label[100]; snprintf(label, sizeof label, "10 bytes of 0x55, destination=buffer+1, value=%d, count=%zu", values[v], n); CASE(label);
        memset(expected + 1, values[v], n);
        POINTER(actual, sizeof actual, actual + 1, my_memset(actual + 1, values[v], n));
        BYTES(expected, actual, 10);
    }
}
#elif EXERCISE == 22
void run_tests(void)
{
    const unsigned char source[] = {0x41, 0, 0xFF, 0x7F, 0x42};
    const unsigned char saved[] = {0x41, 0, 0xFF, 0x7F, 0x42};
    for (size_t n = 0; n <= 5; ++n) {
        unsigned char actual[7], expected[7]; memset(actual, 0x55, 7); memcpy(expected, actual, 7);
        char label[100]; snprintf(label, sizeof label, "source=41 00 FF 7F 42, destination=buffer+1, count=%zu", n); CASE(label);
        memcpy(expected + 1, source, n);
        POINTER(actual, sizeof actual, actual + 1, my_memcpy(actual + 1, source, n));
        BYTES(expected, actual, 7); BYTES(saved, source, 5);
    }
}
#elif EXERCISE == 23
void run_tests(void)
{
    for (size_t from = 0; from <= 4; ++from) for (size_t to = 0; to <= 4; ++to) {
        unsigned char actual[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        unsigned char expected[9]; memcpy(expected, actual, 9);
        char label[100]; snprintf(label, sizeof label, "buffer=00 01 02 03 04 05 06 07 08, from=%zu, to=%zu, count=5", from, to); CASE(label);
        memmove(expected + to, expected + from, 5);
        POINTER(actual, sizeof actual, actual + to, my_memmove(actual + to, actual + from, 5));
        BYTES(expected, actual, 9);
    }
    char a[] = "abc", b[] = "XXX";
    CASE("separate buffers: source=abc, count=4");
    POINTER(b, sizeof b, b, my_memmove(b, a, 4)); BYTES(a, b, 4);
    CASE("zero bytes, valid pointers");
    POINTER(b, sizeof b, b, my_memmove(b, a, 0)); BYTES("abc", b, 4);
}
#elif EXERCISE == 24
void run_tests(void)
{
    const unsigned char a[] = {0x41, 0, 0xFF, 0x20};
    const unsigned char b[] = {0x41, 0, 0x7F, 0x30};
    for (size_t n = 0; n <= 4; ++n) {
        char label[100]; snprintf(label, sizeof label, "a=41 00 FF 20, b=41 00 7F 30, count=%zu; both orders", n); CASE(label);
        INT(sign_of(memcmp(a, b, n)), sign_of(my_memcmp(a, b, n)));
        INT(sign_of(memcmp(b, a, n)), sign_of(my_memcmp(b, a, n)));
        INT(0, my_memcmp(a, a, n));
    }
}
#elif EXERCISE == 25
void run_tests(void)
{
    unsigned char a[] = {0x41, 0, 0xFF, 0x41, 0x42};
    const int targets[] = {0x41, 0, 0xFF, -1, 0x141, 'x', 0x42};
    for (size_t i = 0; i < 7; ++i) for (size_t n = 0; n <= 5; ++n) {
        char label[100]; snprintf(label, sizeof label, "bytes=41 00 FF 41 42, value=%d, count=%zu", targets[i], n); CASE(label);
        POINTER(a, sizeof a, memchr(a, targets[i], n), my_memchr(a, targets[i], n));
    }
}
#elif EXERCISE == 26
void run_tests(void)
{
    const char *haystacks[] = {"hello world", "aaaaab", "banana", "abc", "", "abc", "abc"};
    const char *needles[] = {"world", "aaab", "ana", "abcd", "", "", "x"};
    for (size_t i = 0; i < 7; ++i) {
        char h[32]; strcpy(h, haystacks[i]);
        char label[100]; snprintf(label, sizeof label, "haystack=\"%s\", needle=\"%s\"", h, needles[i]); CASE(label);
        POINTER(h, sizeof h, strstr(h, needles[i]), my_strstr(h, needles[i])); STRING(haystacks[i], h);
    }
}
#elif EXERCISE >= 27 && EXERCISE <= 29
void run_tests(void)
{
    const char *strings[] = {"123abc", "hello,world", "", "abc", "abc", "aaaa", "baba"};
    const char *sets[] = {"0123456789", ",;", "abc", "", "abc", "a", "aab"};
    for (size_t i = 0; i < 7; ++i) {
        char s[32]; strcpy(s, strings[i]);
        char label[100]; snprintf(label, sizeof label, "string=\"%s\", set=\"%s\"", s, sets[i]); CASE(label);
#if EXERCISE == 27
        UINT(strspn(s, sets[i]), my_strspn(s, sets[i]));
#elif EXERCISE == 28
        UINT(strcspn(s, sets[i]), my_strcspn(s, sets[i]));
#else
        POINTER(s, sizeof s, strpbrk(s, sets[i]), my_strpbrk(s, sets[i]));
#endif
        STRING(strings[i], s);
    }
}
#elif EXERCISE == 30
void run_tests(void)
{
    char a[] = ",red;;green,blue,", *state = NULL;
    CASE("string=\",red;;green,blue,\", delimiters=\",;\", first call");
    char *p = next_token(a, ",;", &state); POINTER(a, sizeof a, a + 1, p); STRING("red", p);
    CASE("continue same scan"); p = next_token(NULL, ",;", &state); POINTER(a, sizeof a, a + 6, p); STRING("green", p);
    CASE("continue same scan"); p = next_token(NULL, ",;", &state); POINTER(a, sizeof a, a + 12, p); STRING("blue", p);
    CASE("exhausted scan, twice"); STRING(NULL, next_token(NULL, ",;", &state));
    POINTER(NULL, 0, NULL, state); STRING(NULL, next_token(NULL, ",;", &state));
    char b[] = "a:b,c", c[] = "x/y", *bs = NULL, *cs = NULL;
    CASE("interleave a:b,c and x/y with separate states; change first delimiter set");
    STRING("a", next_token(b, ":", &bs)); STRING("x", next_token(c, "/", &cs));
    STRING("b", next_token(NULL, ",", &bs)); STRING("y", next_token(NULL, "/", &cs));
    STRING("c", next_token(NULL, "", &bs));
    char empty[] = "", all[] = ",,,", whole[] = "whole";
    CASE("empty string, all delimiters, and empty delimiter set");
    STRING(NULL, next_token(empty, ",", &state)); POINTER(NULL, 0, NULL, state);
    STRING(NULL, next_token(all, ",", &state)); POINTER(NULL, 0, NULL, state);
    p = next_token(whole, "", &state); POINTER(whole, sizeof whole, whole, p); STRING("whole", p);
}
#endif
