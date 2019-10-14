include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

# Local headers to build against and distribute but not to install
# since they are not part of the package.
#
noinst_HEADERS                                                      = \
    test_util.h                                                       \
    $(NULL)

if OPENVC_BUILD_TESTS
# C preprocessor option flags that will apply to all compiled objects in this
# makefile.

AM_CPPFLAGS                                                         = \
    -I$(top_srcdir)/include                                           \
    -I$(top_srcdir)/src                                               \
    -I$(top_srcdir)/src/core                                          \
    $(NULL)

if OPENVC_EXAMPLES_POSIX
AM_CPPFLAGS                                                        += \
    -I$(top_srcdir)/examples/platforms                                \
    $(NULL)
endif

COMMON_LDADD                                                        = \
    $(top_builddir)/src/core/libopenvc-core.a                         \
    -lpthread                                                         \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
COMMON_LDADD                                                       += \
    $(top_builddir)/third_party/mbedtls/libmbedcrypto.a               \
    $(NULL)
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

check_PROGRAMS                                                      = \
    test-heap                                                         \
    test-timer                                                        \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
check_PROGRAMS                                                     += \
    test-aes                                                          \
    $(NULL)
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

# Test applications and scripts that should be built and run when the
# 'check' target is run.

TESTS                                                               = \
    $(check_PROGRAMS)                                                 \
    $(NULL)

# Source, compiler, and linker options for test programs.

test_heap_LDADD             = $(COMMON_LDADD)
test_heap_SOURCES           = test_platform.cpp test_heap.cpp

if OPENVC_ENABLE_BUILTIN_MBEDTLS
test_aes_LDADD              = $(COMMON_LDADD)
test_aes_SOURCES            = test_platform.cpp test_aes.cpp
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

test_timer_LDADD            = $(COMMON_LDADD)
test_timer_SOURCES          = test_platform.cpp test_timer.cpp

PRETTY_FILES                                                        = \
    $(test_heap_SOURCES)                                              \
    $(test_timer_SOURCES)                                             \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
PRETTY_FILES                                                       += \
    $(test_aes_SOURCES)                                               \
    $(NULL)
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

if OPENVC_BUILD_COVERAGE
CLEANFILES                   = $(wildcard *.gcda *.gcno)
endif # OPENVC_BUILD_COVERAGE
endif # OPENVC_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
