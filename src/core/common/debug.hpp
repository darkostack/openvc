#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include "openvc-core-config.h"

#include <ctype.h>
#include <stdio.h>
#include "utils/wrap_string.h"

#if defined(OPENVC_TARGET_DARWIN) || defined(OPENVC_TARGET_LINUX)

#include <assert.h>

#elif defined(_KERNEL_MODE)

#include <wdm.h>

#define assert(exp) ((!(exp)) ? (RtlAssert(#exp, __FILE__, __LINE__, NULL), FALSE) : TRUE)

#elif defined(_WIN32)

#include <assert.h>

#elif OPENVC_CONFIG_PLATFORM_ASSERT_MANAGEMENT

#include "openvc/platform/misc.h"

/**
 * Allow the build system to provide a custom file name.
 *
 */
#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

#define assert(cond)                               \
    do                                             \
    {                                              \
        if (!(cond))                               \
        {                                          \
            vcPlatAssertFail(FILE_NAME, __LINE__); \
            while (1)                              \
            {                                      \
            }                                      \
        }                                          \
    } while (0)

#else

#define assert(cond)  \
    do                \
    {                 \
        if (!(cond))  \
        {             \
            while (1) \
            {         \
            }         \
        }             \
    } while (0)

#endif

#endif // DEBUG_HPP_
