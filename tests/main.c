#include "support.h"
#include <stdio.h>

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Exercise %d\n", EXERCISE);
    run_tests();
    return test_summary();
}
