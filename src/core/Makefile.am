include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                       = \
    libopenvc-core.a                  \
    $(NULL)

CPPFLAGS_COMMON                     = \
    -I$(top_srcdir)/include           \
    $(OPENVC_TARGET_DEFINES)          \
    $(NULL)

libopenvc_core_a_CPPFLAGS           = \
    $(CPPFLAGS_COMMON)                \
    $(NULL)

SOURCE_COMMON                       = \
    api/instance_api.cpp              \
    api/tasklet_api.cpp               \
    common/instance.cpp               \
    common/locator.cpp                \
    common/logging.cpp                \
    common/string.cpp                 \
    common/tasklet.cpp                \
    common/timer.cpp                  \
    crypto/aes_ccm.cpp                \
    crypto/aes_ecb.cpp                \
    crypto/hmac_sha256.cpp            \
    crypto/mbedtls.cpp                \
    crypto/sha256.cpp                 \
    utils/heap.cpp                    \
    utils/missing_strlcat.c           \
    utils/missing_strlcpy.c           \
    utils/missing_strnlen.c           \
    $(NULL)

libopenvc_core_a_SOURCES            = \
    $(SOURCE_COMMON)                  \
    $(NULL)

HEADERS_COMMON                      = \
    openvc-core-config.h              \
    openvc-core-default-config.h      \
    common/code_utils.hpp             \
    common/debug.hpp                  \
    common/encoding.hpp               \
    common/instance.hpp               \
    common/locator.hpp                \
    common/logging.hpp                \
    common/new.hpp                    \
    common/owner-locator.hpp          \
    common/random.hpp                 \
    common/string.hpp                 \
    common/tasklet.hpp                \
    common/timer.hpp                  \
    crypto/aes_ccm.hpp                \
    crypto/aes_ecb.hpp                \
    crypto/hmac_sha256.hpp            \
    crypto/mbedtls.hpp                \
    crypto/sha256.hpp                 \
    utils/heap.hpp                    \
    utils/wrap_stdbool.h              \
    utils/wrap_stdint.h               \
    utils/wrap_string.h               \
    $(NULL)

noinst_HEADERS                      = \
    $(HEADERS_COMMON)                 \
    $(NULL)

PRETTY_FILES                        = \
    $(HEADERS_COMMON)                 \
    $(SOURCES_COMMON)                 \
    $(NULL)

if OPENVC_BUILD_COVERAGE
Dash                                = -
CLEANFILES                          = $(shell find $(top_builddir)/src/core $(Dash)name "*.gcda" $(Dash)o $(Dash)name "*.gcno")
endif # OPENVC_BUILD_COVERAGE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
