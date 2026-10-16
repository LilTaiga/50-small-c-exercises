#include "support.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

static size_t checks;
static size_t failures;
static size_t cases;

void test_case(const char *input)
{
    printf("\n  Case %zu | input: %s\n", ++cases, input);
    fflush(stdout);
}

static void failure(const char *expression, int line)
{
    ++failures;
    printf("    FAIL line %d: %s\n", line, expression);
}

static void character(unsigned char c)
{
    switch (c) {
    case '\0': fputs("\\0", stdout); break;
    case '\n': fputs("\\n", stdout); break;
    case '\r': fputs("\\r", stdout); break;
    case '\t': fputs("\\t", stdout); break;
    case '\v': fputs("\\v", stdout); break;
    case '\f': fputs("\\f", stdout); break;
    case '\\': fputs("\\\\", stdout); break;
    case '"': fputs("\\\"", stdout); break;
    case '\'': fputs("\\'", stdout); break;
    default:
        if (c >= 32 && c <= 126) putchar(c);
        else printf("\\x%02X", (unsigned)c);
    }
}

static void quoted(const char *s)
{
    if (!s) { fputs("NULL", stdout); return; }
    putchar('"');
    size_t i = 0;
    for (; i < 128 && s[i]; ++i) character((unsigned char)s[i]);
    putchar('"');
    if (i == 128) fputs("... (display limited to 128 bytes)", stdout);
}

void test_string_case(const char *input)
{
    printf("\n  Case %zu | input: ", ++cases);
    quoted(input);
    putchar('\n');
    fflush(stdout);
}

static void pair_input(const char *a, const char *b)
{
    printf("\n  Case %zu | input: a=", ++cases);
    quoted(a);
    fputs(", b=", stdout);
    quoted(b);
}

void test_pair_case(const char *a, const char *b)
{
    pair_input(a, b);
    putchar('\n');
    fflush(stdout);
}

void test_bounded_pair_case(const char *a, const char *b, size_t count)
{
    pair_input(a, b);
    printf(", count=%zu\n", count);
    fflush(stdout);
}

void expect_int(intmax_t expected, intmax_t actual, const char *expression, int line)
{
    ++checks;
    if (expected == actual) return;
    failure(expression, line);
    printf("      expected: %" PRIdMAX, expected);
    if (expected >= 0 && expected <= 255) { fputs(" (byte '", stdout); character((unsigned char)expected); printf("', 0x%02" PRIXMAX ")", (uintmax_t)expected); }
    printf("\n      actual:   %" PRIdMAX, actual);
    if (actual >= 0 && actual <= 255) { fputs(" (byte '", stdout); character((unsigned char)actual); printf("', 0x%02" PRIXMAX ")", (uintmax_t)actual); }
    putchar('\n');
}

static void unsigned_value(uintmax_t value)
{
    printf("%" PRIuMAX " (0x%" PRIXMAX ", 0b", value, value);
    unsigned bits = (unsigned)(sizeof value * CHAR_BIT);
    bool started = false;
    for (unsigned i = bits; i > 0; --i) {
        unsigned bit = (unsigned)((value >> (i - 1)) & 1);
        if (bit || started || i == 1) { putchar('0' + bit); started = true; }
    }
    putchar(')');
}

void expect_uint(uintmax_t expected, uintmax_t actual, const char *expression, int line)
{
    ++checks;
    if (expected == actual) return;
    failure(expression, line);
    fputs("      expected: ", stdout); unsigned_value(expected);
    fputs("\n      actual:   ", stdout); unsigned_value(actual); putchar('\n');
}

static const char *parse_name(ParseResult result)
{
    switch (result) {
    case PARSE_OK: return "PARSE_OK";
    case PARSE_INVALID: return "PARSE_INVALID";
    case PARSE_OVERFLOW: return "PARSE_OVERFLOW";
    default: return "unknown parse status";
    }
}

void expect_parse(ParseResult expected, ParseResult actual, const char *expression, int line)
{
    ++checks;
    if (expected == actual) return;
    failure(expression, line);
    printf("      expected: %s (%d)\n      actual:   %s (%d)\n",
           parse_name(expected), (int)expected, parse_name(actual), (int)actual);
}

void expect_string(const char *expected, const char *actual, const char *expression, int line)
{
    ++checks;
    if ((!expected && !actual) || (expected && actual && strcmp(expected, actual) == 0)) return;
    failure(expression, line);
    fputs("      expected: ", stdout); quoted(expected);
    fputs("\n      actual:   ", stdout); quoted(actual); putchar('\n');
    if (expected && actual) {
        size_t i = 0;
        while (expected[i] && actual[i] && expected[i] == actual[i]) ++i;
        printf("      first differing byte: %zu (expected 0x%02X, actual 0x%02X)\n",
               i, (unsigned)(unsigned char)expected[i], (unsigned)(unsigned char)actual[i]);
    }
}

static void byte_dump(const void *memory, size_t count)
{
    if (!memory) { puts("NULL"); return; }
    const unsigned char *bytes = memory;
    size_t shown = count < 64 ? count : 64;
    for (size_t i = 0; i < shown; ++i) printf("%02X%s", (unsigned)bytes[i], i + 1 == shown ? "" : " ");
    fputs(" |", stdout);
    for (size_t i = 0; i < shown; ++i) putchar(bytes[i] >= 32 && bytes[i] <= 126 ? bytes[i] : '.');
    puts(count > shown ? "| ... (display limited to 64 bytes)" : "|");
}

void expect_bytes(const void *expected, const void *actual, size_t count, const char *expression, int line)
{
    ++checks;
    if (!count || (expected && actual && memcmp(expected, actual, count) == 0)) return;
    failure(expression, line);
    fputs("      expected: ", stdout); byte_dump(expected, count);
    fputs("      actual:   ", stdout); byte_dump(actual, count);
    if (expected && actual) {
        const unsigned char *a = expected, *b = actual;
        size_t i = 0;
        while (i < count && a[i] == b[i]) ++i;
        printf("      first differing byte: %zu (expected 0x%02X, actual 0x%02X)\n",
               i, (unsigned)a[i], (unsigned)b[i]);
    }
}

static void array_dump(const int *values, size_t count)
{
    if (!values) { puts("NULL"); return; }
    putchar('[');
    size_t shown = count < 32 ? count : 32;
    for (size_t i = 0; i < shown; ++i) printf("%s%d", i ? ", " : "", values[i]);
    puts(count > shown ? ", ...]" : "]");
}

void expect_array(const int *expected, const int *actual, size_t count, const char *expression, int line)
{
    ++checks;
    size_t i = 0;
    if (!count) return;
    if (expected && actual) {
        while (i < count && expected[i] == actual[i]) ++i;
        if (i == count) return;
    }
    failure(expression, line);
    fputs("      expected: ", stdout); array_dump(expected, count);
    fputs("      actual:   ", stdout); array_dump(actual, count);
    if (expected && actual) printf("      first differing index: %zu (expected %d, actual %d)\n", i, expected[i], actual[i]);
}

static void pointer_dump(const void *base, size_t extent, const void *pointer)
{
    if (!pointer) { puts("NULL"); return; }
    if (base) {
        const unsigned char *bytes = base;
        for (size_t i = 0; i <= extent; ++i) {
            if (pointer == bytes + i) { printf("buffer + %zu\n", i); return; }
        }
    }
    puts("non-NULL pointer outside the supplied buffer");
}

void expect_pointer(const void *base, size_t extent, const void *expected, const void *actual, const char *expression, int line)
{
    ++checks;
    if (expected == actual) return;
    failure(expression, line);
    fputs("      expected: ", stdout); pointer_dump(base, extent, expected);
    fputs("      actual:   ", stdout); pointer_dump(base, extent, actual);
}

int sign_of(int value) { return (value > 0) - (value < 0); }

int test_summary(void)
{
    printf("\n%s: %zu/%zu checks passed across %zu cases.\n",
           failures ? "FAIL" : "PASS", checks - failures, checks, cases);
    return failures ? 1 : 0;
}
