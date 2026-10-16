#ifndef LEARN_C_TEST_SUPPORT_H
#define LEARN_C_TEST_SUPPORT_H

#include "exercises.h"
#include <stdbool.h>
#include <inttypes.h>

void test_case(const char *input);
void test_string_case(const char *input);
void test_pair_case(const char *a, const char *b);
void expect_int(intmax_t expected, intmax_t actual, const char *expression, int line);
void expect_uint(uintmax_t expected, uintmax_t actual, const char *expression, int line);
void expect_parse(ParseResult expected, ParseResult actual, const char *expression, int line);
void expect_string(const char *expected, const char *actual, const char *expression, int line);
void expect_bytes(const void *expected, const void *actual, size_t count, const char *expression, int line);
void expect_array(const int *expected, const int *actual, size_t count, const char *expression, int line);
void expect_pointer(const void *base, size_t extent, const void *expected, const void *actual, const char *expression, int line);
int test_summary(void);
int sign_of(int value);
void run_tests(void);

#define CASE(input) test_case(input)
#define STRING_CASE(input) test_string_case(input)
#define PAIR_CASE(a, b) test_pair_case((a), (b))
#define INT(expected, actual) expect_int((expected), (actual), #actual, __LINE__)
#define UINT(expected, actual) expect_uint((expected), (actual), #actual, __LINE__)
#define PARSE(expected, actual) expect_parse((expected), (actual), #actual, __LINE__)
#define STRING(expected, actual) expect_string((expected), (actual), #actual, __LINE__)
#define BYTES(expected, actual, count) expect_bytes((expected), (actual), (count), #actual, __LINE__)
#define ARRAY(expected, actual, count) expect_array((expected), (actual), (count), #actual, __LINE__)
#define POINTER(base, extent, expected, actual) expect_pointer((base), (extent), (expected), (actual), #actual, __LINE__)
#define TRUE(actual) INT(1, !!(actual))

#endif
