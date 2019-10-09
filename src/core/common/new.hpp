#ifndef NEW_HPP_
#define NEW_HPP_

#include "openvc-core-config.h"

#include <stddef.h>

#include <openvc/platform/toolchain.h>

inline void *operator new(size_t, void *p) throw()
{
    return p;
}

#endif // NEW_HPP_
