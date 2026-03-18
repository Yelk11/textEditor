#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <string.h>

extern int UnityTestsRun;
extern int UnityTestsFail;

#define UNITY_BEGIN() do { UnityTestsRun = 0; UnityTestsFail = 0; } while (0)
#define UNITY_END()  (UnityTestsFail == 0 ? 0 : 1)

#define UNITY_TEST_ASSERT(condition, message) \
    do { \
        UnityTestsRun++; \
        if (!(condition)) { \
            printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, message); \
            UnityTestsFail++; \
        } \
    } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UNITY_TEST_ASSERT((expected) == (actual), "Expected " #expected " == " #actual)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    UNITY_TEST_ASSERT(strcmp((expected), (actual)) == 0, "Expected strings to match")

#define RUN_TEST(test) \
    do { \
        printf("RUN: %s\n", #test); \
        test(); \
    } while (0)

#endif // UNITY_H
