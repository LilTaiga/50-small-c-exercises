#include "support.h"
#include <limits.h>
#include <stdio.h>

#if EXERCISE >= 1 && EXERCISE <= 6
void run_tests(void)
{
    /* Deliberately include ints outside unsigned-char range: our ASCII-only
     * contract accepts them, unlike the standard ctype interfaces. */
    const int inputs[] = {INT_MIN, -1, 0, 8, 9, 10, 11, 12, 13, 14, 32,
                          '/', '0', '5', '9', ':', '@', 'A', 'M', 'Z', '[',
                          '_', '`', 'a', 'm', 'z', '{', 127, 128, 255, 256, INT_MAX};
    for (size_t i = 0; i < sizeof inputs / sizeof *inputs; ++i) {
        int c = inputs[i];
        CHAR_CASE("ASCII character", c);
#if EXERCISE == 1
        BOOL(c >= '0' && c <= '9', my_isdigit(c));
#elif EXERCISE == 2
        BOOL((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'), my_isalpha(c));
#elif EXERCISE == 3
        BOOL((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'), my_isalnum(c));
#elif EXERCISE == 4
        BOOL(c == ' ' || (c >= '\t' && c <= '\r'), my_isspace(c));
#elif EXERCISE == 5
        CHAR(c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c, my_toupper(c));
#else
        CHAR(c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c, my_tolower(c));
#endif
    }
}
#elif EXERCISE == 7
void run_tests(void)
{
    CASE("-42, 12, 0, -1, INT_MAX, -INT_MAX");
    INT(42, my_abs(-42)); INT(12, my_abs(12)); INT(0, my_abs(0));
    INT(1, my_abs(-1)); INT(INT_MAX, my_abs(INT_MAX)); INT(INT_MAX, my_abs(-INT_MAX));
}
#elif EXERCISE == 8
void run_tests(void)
{
    CASE("value=15, range=[0,10]"); INT(10, clamp_int(15, 0, 10));
    CASE("value=-5, range=[0,10]"); INT(0, clamp_int(-5, 0, 10));
    CASE("values=0,4,10, range=[0,10]");
    INT(0, clamp_int(0, 0, 10)); INT(4, clamp_int(4, 0, 10)); INT(10, clamp_int(10, 0, 10));
    CASE("negative range [-9,-3], equal limits [7,7], full int range");
    INT(-5, clamp_int(-5, -9, -3)); INT(7, clamp_int(-100, 7, 7));
    INT(INT_MIN, clamp_int(INT_MIN, INT_MIN, INT_MAX));
    INT(INT_MAX, clamp_int(INT_MAX, INT_MIN, INT_MAX));
}
#elif EXERCISE == 9
void run_tests(void)
{
    int a = 3, b = 8;
    CASE("a=3, b=8"); swap_int(&a, &b); INT(8, a); INT(3, b);
    a = INT_MIN; b = INT_MAX;
    CASE("a=INT_MIN, b=INT_MAX"); swap_int(&a, &b); INT(INT_MAX, a); INT(INT_MIN, b);
    a = -7; b = -7;
    CASE("both values -7"); swap_int(&a, &b); INT(-7, a); INT(-7, b);
    CASE("both pointers address a=-7"); swap_int(&a, &a); INT(-7, a);
}
#elif EXERCISE == 10
void run_tests(void)
{
    const int a[] = {3, -1, 5, 100};
    const int expected[] = {3, -1, 5, 100};
    CASE("[3,-1,5,100], count=3"); INT(7, array_sum(a, 3)); ARRAY(expected, a, 4);
    const int b[] = {-4, -8};
    CASE("[-4,-8], count=2"); INT(-12, array_sum(b, 2));
    CASE("[INT_MAX], count=1"); const int c[] = {INT_MAX}; INT(INT_MAX, array_sum(c, 1));
    CASE("NULL, count=0"); INT(0, array_sum(NULL, 0));
}
#endif
