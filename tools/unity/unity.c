/* Minimal Unity implementation */
#include "unity.h"
#include <stdio.h>

int _unity_failures = 0;
int _unity_tests    = 0;

int UNITY_BEGIN(void) {
    _unity_failures = 0;
    _unity_tests    = 0;
    printf("-----------------------\n");
    return 0;
}

int UNITY_END(void) {
    printf("-----------------------\n");
    printf("%d Tests %d Failures 0 Ignored\n", _unity_tests, _unity_failures);
    if (_unity_failures == 0) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }
    return _unity_failures;
}

void _unity_run_test(void (*test)(void), const char *name,
                     const char *file, int line) {
    int failures_before = _unity_failures;
    _unity_tests++;
    test();
    if (_unity_failures == failures_before) {
        printf("%s:%d:%s:PASS\n", file, line, name);
    }
}
