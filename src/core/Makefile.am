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
    common/logging.cpp                \
    common/string.cpp                 \
    common/tasklet.cpp                \
    common/timer.cpp                  \
    utils/heap.cpp                    \
    utils/missing_strlcat.c           \
    utils/missing_strlcpy.c           \
    utils/missing_strnlen.c           \
    utils/parse_cmdline.cpp           \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
SOURCE_COMMON                      += \
    crypto/aes_ccm.cpp                \
    crypto/aes_ecb.cpp                \
    crypto/ecdsa.cpp                  \
    crypto/hmac_sha256.cpp            \
    crypto/mbedtls.cpp                \
    crypto/sha256.cpp                 \
    $(NULL)
endif

libopenvc_core_a_SOURCES            = \
    $(SOURCE_COMMON)                  \
    $(NULL)

HEADERS_COMMON                      = \
    openvc-core-config.h              \
    common/code_utils.hpp             \
    common/debug.hpp                  \
    common/encoding.hpp               \
    common/instance.hpp               \
    common/locator-getters.hpp        \
    common/locator.hpp                \
    common/logging.hpp                \
    common/new.hpp                    \
    common/random.hpp                 \
    common/string.hpp                 \
    common/tasklet.hpp                \
    common/time.hpp                   \
    common/timer.hpp                  \
    utils/heap.hpp                    \
    utils/parse_cmdline.hpp           \
    utils/static_assert.hpp           \
    utils/wrap_string.h               \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
HEADERS_COMMON                     += \
    crypto/aes_ccm.hpp                \
    crypto/aes_ecb.hpp                \
    crypto/ecdsa.hpp                  \
    crypto/hmac_sha256.hpp            \
    crypto/mbedtls.hpp                \
    crypto/sha256.hpp                 \
    $(NULL)
endif

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
