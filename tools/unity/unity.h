/* =========================================================================
   unity.h  -  Minimal Unity-compatible test framework (header-only subset)
   Provides the macros used by ArduinoMock tests when running under plain g++
   without a full PlatformIO installation.
   ========================================================================= */
#pragma once
#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
   Internal state
   ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

extern int _unity_failures;
extern int _unity_tests;

int  UNITY_BEGIN(void);
int  UNITY_END(void);
void _unity_run_test(void (*test)(void), const char *name, const char *file, int line);

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------
   RUN_TEST macro
   ------------------------------------------------------------------------- */
#define RUN_TEST(t)  _unity_run_test(t, #t, __FILE__, __LINE__)

/* -------------------------------------------------------------------------
   Assertion helpers
   ------------------------------------------------------------------------- */
#define _UNITY_FAIL(file, line, msg) \
    do { \
        fprintf(stderr, "%s:%d: FAIL: %s\n", file, line, msg); \
        _unity_failures++; \
    } while (0)

/* -------------------------------------------------------------------------
   Assertion macros (Unity-compatible names)
   ------------------------------------------------------------------------- */
#define TEST_PASS() \
    do { /* intentionally empty */ } while (0)

#define TEST_FAIL_MESSAGE(msg) \
    _UNITY_FAIL(__FILE__, __LINE__, msg)

#define TEST_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            _UNITY_FAIL(__FILE__, __LINE__, "TEST_ASSERT(" #cond ") failed"); \
        } \
    } while (0)

#define TEST_ASSERT_TRUE(cond)  TEST_ASSERT(cond)
#define TEST_ASSERT_FALSE(cond) TEST_ASSERT(!(cond))

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((long long)(expected) != (long long)(actual)) { \
            char _buf[256]; \
            snprintf(_buf, sizeof(_buf), \
                     "Expected %lld but got %lld", \
                     (long long)(expected), (long long)(actual)); \
            _UNITY_FAIL(__FILE__, __LINE__, _buf); \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_UINT(expected, actual) \
    do { \
        if ((unsigned long long)(expected) != (unsigned long long)(actual)) { \
            char _buf[256]; \
            snprintf(_buf, sizeof(_buf), \
                     "Expected %llu but got %llu", \
                     (unsigned long long)(expected), (unsigned long long)(actual)); \
            _UNITY_FAIL(__FILE__, __LINE__, _buf); \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        const char *_e = (expected); \
        const char *_a = (actual); \
        if (!_e || !_a || strcmp(_e, _a) != 0) { \
            char _buf[256]; \
            snprintf(_buf, sizeof(_buf), \
                     "Expected \"%s\" but got \"%s\"", \
                     _e ? _e : "(null)", _a ? _a : "(null)"); \
            _UNITY_FAIL(__FILE__, __LINE__, _buf); \
        } \
    } while (0)

#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual) \
    do { \
        float _diff = (float)(actual) - (float)(expected); \
        if (_diff < 0) _diff = -_diff; \
        if (_diff > (float)(delta)) { \
            char _buf[256]; \
            snprintf(_buf, sizeof(_buf), \
                     "Expected %f +/- %f but got %f", \
                     (double)(expected), (double)(delta), (double)(actual)); \
            _UNITY_FAIL(__FILE__, __LINE__, _buf); \
        } \
    } while (0)

#define TEST_ASSERT_NULL(ptr) \
    TEST_ASSERT((ptr) == NULL)

#define TEST_ASSERT_NOT_NULL(ptr) \
    TEST_ASSERT((ptr) != NULL)

#endif /* UNITY_H */
