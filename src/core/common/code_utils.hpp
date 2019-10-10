#ifndef CODE_UTILS_HPP_
#define CODE_UTILS_HPP_

#include "openvc-core-config.h"

#include <stdbool.h>

#include "utils/static_assert.hpp"

#define VC_ARRAY_LENGTH(aArray) (sizeof(aArray) / sizeof(aArray[0]))

// Calculates the aligned variable size.
#define vcALIGNED_VAR_SIZE(aSize, aAlignType) (((aSize) + (sizeof(aAlignType) - 1)) / sizeof(aAlignType))

// Allocate the structure using "raw" storage.
#define vcDEFINE_ALIGNED_VAR(aName, aSize, aAlignType) \
    aAlignType aName[(((aSize) + (sizeof(aAlignType) - 1)) / sizeof(aAlignType))]

#define SuccessOrExit(aStatus) \
    do                         \
    {                          \
        if ((aStatus) != 0)    \
        {                      \
            goto exit;         \
        }                      \
    } while (false)

#define VerifyOrExit(aCondition, ...) \
    do                                \
    {                                 \
        if (!(aCondition))            \
        {                             \
            __VA_ARGS__;              \
            goto exit;                \
        }                             \
    } while (false)

#define ExitNow(...) \
    do               \
    {                \
        __VA_ARGS__; \
        goto exit;   \
    } while (false)

#define IgnoreReturnValue(aStatement) \
    do                                \
    {                                 \
        if (aStatement)               \
        {                             \
        }                             \
    } while (false)

#endif // CODE_UTILS_HPP_
