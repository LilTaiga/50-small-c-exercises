#include "support.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#if EXERCISE == 31
void run_tests(void)
{
    const char *inputs[] = {"  -42 cats", "123abc", "\t\n\v\f\r +17", "", "+", "-", "abc", "--1", "+-1", "0007", "-0", "  - 2"};
    const int expected[] = {-42, 123, 17, 0, 0, 0, 0, 0, 0, 7, 0, 0};
    for (size_t i = 0; i < 12; ++i) {
        STRING_CASE(inputs[i]);
        INT(expected[i], my_atoi(inputs[i]));
    }
    char limit[80]; snprintf(limit, sizeof limit, "%d", INT_MIN); CASE(limit); INT(INT_MIN, my_atoi(limit));
    snprintf(limit, sizeof limit, "%d", INT_MAX); CASE(limit); INT(INT_MAX, my_atoi(limit));
}
#elif EXERCISE == 32
void run_tests(void)
{
    const char *valid[] = {"42", "-42", "+17", "0007", "-0"};
    const int expected[] = {42, -42, 17, 7, 0};
    for (size_t i = 0; i < 5; ++i) {
        int value = 1234; STRING_CASE(valid[i]); PARSE(PARSE_OK, parse_int(valid[i], &value)); INT(expected[i], value);
    }
    const char *invalid[] = {"", "+", "-", " 1", "1 ", "1x", "--1", "+-2"};
    for (size_t i = 0; i < 8; ++i) {
        int value = 1234; STRING_CASE(invalid[i]); PARSE(PARSE_INVALID, parse_int(invalid[i], &value)); INT(1234, value);
    }
    char text[100]; int value = 1234;
    snprintf(text, sizeof text, "%d", INT_MIN); CASE(text); PARSE(PARSE_OK, parse_int(text, &value)); INT(INT_MIN, value);
    snprintf(text, sizeof text, "%d", INT_MAX); CASE(text); PARSE(PARSE_OK, parse_int(text, &value)); INT(INT_MAX, value);
    snprintf(text, sizeof text, "%d0", INT_MAX); CASE(text); value = 1234;
    PARSE(PARSE_OVERFLOW, parse_int(text, &value)); INT(1234, value);
    snprintf(text, sizeof text, "%d0", INT_MIN); CASE(text);
    PARSE(PARSE_OVERFLOW, parse_int(text, &value)); INT(1234, value);
    snprintf(text, sizeof text, "%ju", (uintmax_t)INT_MAX + 1); CASE(text);
    PARSE(PARSE_OVERFLOW, parse_int(text, &value)); INT(1234, value);
    snprintf(text, sizeof text, "-%ju", (uintmax_t)(-(INT_MIN + 1)) + 2); CASE(text);
    PARSE(PARSE_OVERFLOW, parse_int(text, &value)); INT(1234, value);
    CASE("syntactically invalid number after a long digit sequence");
    PARSE(PARSE_INVALID, parse_int("999999999999999999999999999999999999x", &value)); INT(1234, value);
}
#elif EXERCISE == 33
void run_tests(void)
{
    const int values[] = {42, -17, 0, 1, -1, INT_MIN, INT_MAX};
    for (size_t i = 0; i < 7; ++i) {
        char text[100]; snprintf(text, sizeof text, "%d", values[i]);
        size_t needed = strlen(text) + 1;
        for (size_t capacity = 0; capacity <= needed + 1; ++capacity) {
            char actual[100], expected[100]; memset(actual, 0x55, 100); memcpy(expected, actual, 100);
            char label[100]; snprintf(label, sizeof label, "value=%d, capacity=%zu", values[i], capacity); CASE(label);
            int status = int_to_string(values[i], actual, capacity);
            INT(capacity >= needed ? 0 : -1, status);
            if (capacity >= needed) memcpy(expected, text, needed);
            BYTES(expected, actual, 100);
        }
    }
    CASE("value=42, destination=NULL, capacity=0"); INT(-1, int_to_string(42, NULL, 0));
}
#elif EXERCISE == 34
void run_tests(void)
{
    const char *valid[] = {"FF", "ff", "1011", "z", "000", "17"};
    const unsigned bases[] = {16, 16, 2, 36, 10, 8};
    const unsigned expected[] = {255, 255, 11, 35, 0, 15};
    for (size_t i = 0; i < 6; ++i) {
        unsigned value = 77; char label[80]; snprintf(label, sizeof label, "string=%s, base=%u", valid[i], bases[i]); CASE(label);
        PARSE(PARSE_OK, parse_uint_base(valid[i], bases[i], &value)); UINT(expected[i], value);
    }
    const char *invalid[] = {"", "2", "0xFF", "-1", "+1", " 1", "1 ", "1"};
    const unsigned bad_bases[] = {10, 2, 16, 10, 10, 10, 10, 37};
    for (size_t i = 0; i < 8; ++i) {
        unsigned value = 77; char label[80]; snprintf(label, sizeof label, "string=%s, base=%u", invalid[i], bad_bases[i]); CASE(label);
        PARSE(PARSE_INVALID, parse_uint_base(invalid[i], bad_bases[i], &value)); UINT(77, value);
    }
    unsigned value = 77; CASE("string=1, base=1"); PARSE(PARSE_INVALID, parse_uint_base("1", 1, &value)); UINT(77, value);
    char text[100]; snprintf(text, sizeof text, "%u", UINT_MAX); CASE(text);
    PARSE(PARSE_OK, parse_uint_base(text, 10, &value)); UINT(UINT_MAX, value);
    snprintf(text, sizeof text, "%u0", UINT_MAX); value = 77; CASE(text);
    PARSE(PARSE_OVERFLOW, parse_uint_base(text, 10, &value)); UINT(77, value);
    /* Decimal increment avoids assuming a wider unsigned integer type. */
    snprintf(text, sizeof text, "%u", UINT_MAX);
    size_t end = strlen(text);
    while (end && text[end - 1] == '9') text[--end] = '0';
    if (end) ++text[end - 1];
    else { size_t length = strlen(text); memmove(text + 1, text, length + 1); text[0] = '1'; }
    CASE(text); PARSE(PARSE_OVERFLOW, parse_uint_base(text, 10, &value)); UINT(77, value);
    CASE("base=10, long digit sequence followed by x");
    PARSE(PARSE_INVALID, parse_uint_base("999999999999999999999999999999999999x", 10, &value)); UINT(77, value);
}
#elif EXERCISE == 35
void run_tests(void)
{
    const char *inputs[] = {"hello", "", "a\nb"};
    for (size_t i = 0; i < 3; ++i) {
        char source[16]; strcpy(source, inputs[i]); STRING_CASE(inputs[i]);
        char *copy = my_strdup(source); STRING(inputs[i], copy);
        if (copy) { TRUE(copy != source); if (*copy) copy[0] = '!'; STRING(inputs[i], source); free(copy); }
    }
}
#elif EXERCISE == 36
void run_tests(void)
{
    CASE("count=4, size=3"); unsigned char *p = my_calloc(4, 3);
    const unsigned char zero[12] = {0}; BYTES(zero, p, 12); free(p);
    CASE("count=2, size=sizeof(int); malloc alignment"); int *ints = my_calloc(2, sizeof *ints);
    TRUE(ints != NULL); if (ints) { INT(0, ints[0]); INT(0, ints[1]); ints[0] = 42; free(ints); }
    CASE("zero count and zero size"); POINTER(NULL, 0, NULL, my_calloc(0, 8)); POINTER(NULL, 0, NULL, my_calloc(8, 0));
    CASE("multiplication overflow: SIZE_MAX * 2 and (SIZE_MAX/2+1) * 2");
    POINTER(NULL, 0, NULL, my_calloc(SIZE_MAX, 2)); POINTER(NULL, 0, NULL, my_calloc(SIZE_MAX / 2 + 1, 2));
}
#elif EXERCISE == 37
void run_tests(void)
{
    int source[] = {3, -1, INT_MAX}; const int expected[] = {3, -1, INT_MAX};
    CASE("[3,-1,INT_MAX], count=3"); int *p = array_clone(source, 3); ARRAY(expected, p, 3);
    if (p) { TRUE(p != source); p[0] = 99; ARRAY(expected, source, 3); free(p); }
    CASE("NULL, count=0"); POINTER(NULL, 0, NULL, array_clone(NULL, 0));
    CASE("NULL, count=SIZE_MAX/sizeof(int)+1; must reject before reading");
    POINTER(NULL, 0, NULL, array_clone(NULL, SIZE_MAX / sizeof(int) + 1));
}
#elif EXERCISE == 38
void run_tests(void)
{
    const size_t starts[] = {6, 0, 11, 2, 12, 0};
    const size_t lengths[] = {5, 11, 0, 0, 0, SIZE_MAX};
    const char *expected[] = {"world", "hello world", "", "", NULL, NULL};
    char source[] = "hello world";
    for (size_t i = 0; i < 6; ++i) {
        char label[100]; snprintf(label, sizeof label, "source=hello world, start=%zu, length=%zu", starts[i], lengths[i]); CASE(label);
        char *p = string_slice(source, starts[i], lengths[i]); STRING(expected[i], p);
        if (p) { TRUE(p != source + starts[i]); free(p); }
        STRING("hello world", source);
    }
}
#elif EXERCISE == 39
void run_tests(void)
{
    const char *words[] = {"red", "green", "blue"};
    CASE("[red,green,blue], separator=, "); char *p = string_join(words, 3, ", "); STRING("red, green, blue", p); free(p);
    CASE("one string, separator=/"); p = string_join(words, 1, "/"); STRING("red", p); free(p);
    CASE("no strings"); p = string_join(NULL, 0, "/"); STRING("", p); free(p);
    const char *empty[] = {"", "x", ""};
    CASE("[empty,x,empty], separator=::"); p = string_join(empty, 3, "::"); STRING("::x::", p); free(p);
    CASE("[red,green,blue], empty separator"); p = string_join(words, 3, ""); STRING("redgreenblue", p); free(p);
}
#elif EXERCISE == 40
void run_tests(void)
{
    const char *inputs[] = {"red,green,blue", ",a,,b,", "", "whole"};
    const char *expected[][5] = {{"red", "green", "blue"}, {"", "a", "", "b", ""}, {""}, {"whole"}};
    const size_t counts[] = {3, 5, 1, 1};
    for (size_t i = 0; i < 4; ++i) {
        char source[32]; strcpy(source, inputs[i]); STRING_CASE(inputs[i]); size_t count = 999;
        char **fields = string_split(source, ',', &count); UINT(counts[i], count); TRUE(fields != NULL);
        if (fields && count == counts[i]) {
            for (size_t j = 0; j < count; ++j) STRING(expected[i][j], fields[j]);
            if (fields[0] && fields[0][0]) fields[0][0] = '!';
            STRING(inputs[i], source); string_split_free(fields, count);
        }
    }
    CASE("cleanup(NULL,0)"); string_split_free(NULL, 0);
}
#endif
