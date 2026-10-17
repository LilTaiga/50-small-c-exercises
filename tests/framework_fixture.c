/* Fixtures test the feedback machinery only. No exercise solutions live here. */
#include "support.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    const char *mode = getenv("FRAMEWORK_MODE");
    if (!mode) mode = "pass";
    CASE("framework fixture");
    if (!strcmp(mode, "crash")) { raise(SIGSEGV); return 2; }
    if (!strcmp(mode, "hang")) { for (;;) { } }
    if (!strcmp(mode, "exit")) return 7;
    if (!strcmp(mode, "partial")) {
        fputs("    FAIL line 0: output without a final newline", stdout);
        return 1;
    }
    if (!strcmp(mode, "closed-output")) {
        fclose(stdout);
        for (;;) { }
    }
    if (!strcmp(mode, "flood")) {
        for (;;) puts("output from a runaway implementation");
    }
    if (!strcmp(mode, "bool-fail")) {
        BOOL(true, false); BOOL(false, 17); TRUE(0);
        return test_summary();
    }
    if (!strcmp(mode, "bool-pass")) {
        BOOL(true, 17); BOOL(true, -9); BOOL(false, 0); TRUE(-1);
        return test_summary();
    }
    if (!strcmp(mode, "mixed")) {
        CASE("passing before"); INT(1, 1);
        CASE("failing middle"); INT(1, 2); INT(3, 4);
        CASE("passing after"); INT(5, 5);
        return test_summary();
    }
    if (!strcmp(mode, "mixed-crash") || !strcmp(mode, "mixed-hang")) {
        CASE("passing before"); INT(1, 1);
        CASE("failing middle"); INT(1, 2);
        CASE("active input before interruption");
        if (!strcmp(mode, "mixed-crash")) { raise(SIGSEGV); return 2; }
        for (;;) { }
    }
    if (!strcmp(mode, "escaped-input")) {
        STRING_CASE("a\nb\t\"\\"); INT(1, 2);
        PAIR_CASE("\xFF", "\x7F"); INT(3, 4);
        BOUNDED_PAIR_CASE("a\nb", "a\tb", 2); INT(5, 6);
        return test_summary();
    }
    if (!strcmp(mode, "stderr")) {
        fputs("diagnostic on stderr\n", stderr);
        return test_summary();
    }
    if (!strcmp(mode, "fail")) {
        unsigned char actual[] = {0x41, 0, 0xFF}, expected[] = {0x42, 0x0A, 0x7F};
        int a[] = {1, 2, 3}, b[] = {1, 9, 3};
        INT('A', 'B'); UINT(0xA5, 0xF0); STRING("a\n\t\\\"", "a\r\x01");
        BYTES(expected, actual, 3); ARRAY(a, b, 3);
        POINTER(actual, sizeof actual, actual + 1, actual + 2);
        POINTER(actual, sizeof actual, NULL, actual);
        STRING("not null", NULL); STRING(NULL, "not null");
        PARSE(PARSE_OK, PARSE_OVERFLOW);
        unsigned char long_expected[80] = {0}, long_actual[80] = {0};
        long_actual[79] = 0xFF;
        BYTES(long_expected, long_actual, sizeof long_actual);
        int long_a[40] = {0}, long_b[40] = {0}; long_b[39] = 42;
        ARRAY(long_a, long_b, 40);
        return test_summary();
    }
    char s[] = "a\nb"; unsigned char bytes[] = {0, 0xFF}; int values[] = {1, -2};
    STRING_CASE(s); PAIR_CASE("\xFF", "\x7F");
    INT(-2, -2); UINT(UINTMAX_MAX, UINTMAX_MAX); STRING(s, s);
    STRING(NULL, NULL); BYTES(bytes, bytes, 2); BYTES(NULL, NULL, 0);
    ARRAY(values, values, 2); ARRAY(NULL, NULL, 0);
    POINTER(s, sizeof s, s + 1, s + 1); POINTER(NULL, 0, NULL, NULL);
    PARSE(PARSE_OK, PARSE_OK);
    return test_summary();
}
