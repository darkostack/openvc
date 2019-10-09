#ifndef OPENVC_ERROR_H_
#define OPENVC_ERROR_H_

#include <openvc/platform/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum vcError {
    VC_ERROR_NONE             = 0,
    VC_ERROR_FAILED           = 1,
    VC_ERROR_DROP             = 2,
    VC_ERROR_NO_BUFS          = 3,
    VC_ERROR_BUSY             = 4,
    VC_ERROR_PARSE            = 5,
    VC_ERROR_INVALID_ARGS     = 6,
    VC_ERROR_SECURITY         = 7,
    VC_ERROR_ABORT            = 8,
    VC_ERROR_NOT_IMPLEMENTED  = 9,
    VC_ERROR_INVALID_STATE    = 10,
    VC_ERROR_NOT_FOUND        = 11,
    VC_ERROR_ALREADY          = 12,
    VC_ERROR_NOT_CAPABLE      = 13,
    VC_ERROR_RESPONSE_TIMEOUT = 14,
    VC_ERROR_DISABLED_FEATURE = 15,
    VC_ERROR_GENERIC          = 255,
} vcError;

const char *vcErrorToString(vcError aError);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_ERROR_H_
