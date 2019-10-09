#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Enable main functions
#define ENABLE_TEST_MAIN

#define SuccessOrQuit(ERR, MSG)                                                    \
    do                                                                             \
    {                                                                              \
        if ((ERR) != NS_ERROR_NONE)                                                \
        {                                                                          \
            fprintf(stderr, "\nFAILED %s:%d - %s\n", __FUNCTION__, __LINE__, MSG); \
            exit(-1);                                                              \
        }                                                                          \
    } while (false)

#define VerifyOrQuit(TST, MSG)                                                     \
    do                                                                             \
    {                                                                              \
        if (!(TST))                                                                \
        {                                                                          \
            fprintf(stderr, "\nFAILED %s:%d - %s\n", __FUNCTION__, __LINE__, MSG); \
            exit(-1);                                                              \
        }                                                                          \
    } while (false)

#define CompileTimeAssert(COND, MSG)

#define Log(aFormat, ...) printf(aFormat "\n", ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // TEST_UTIL_H_
