include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

EXTRA_DIST                              = \
    mbedtls-config.h                      \
    repo/include                          \
    $(NULL)

lib_LIBRARIES                           = libmbedcrypto.a

# Do not enable -Wconversion for mbedtls
override CFLAGS                        := $(filter-out -Wconversion,$(CFLAGS))
override CXXFLAGS                      := $(filter-out -Wconversion,$(CXXFLAGS))

# Do not enable -pedantic-errors for mbedtls
override CFLAGS                        := $(filter-out -pedantic-errors,$(CFLAGS))
override CXXFLAGS                      := $(filter-out -pedantic-errors,$(CXXFLAGS))

MBEDTLS_SRCDIR                          = $(top_srcdir)/third_party/mbedtls/repo

libmbedcrypto_a_CPPFLAGS                = \
    -I$(top_srcdir)/include               \
    -I$(top_srcdir)/src/core              \
    -I$(MBEDTLS_SRCDIR)/include           \
    $(MBEDTLS_CPPFLAGS)                   \
    $(NULL)

libmbedcrypto_a_SOURCES                 = \
    repo/library/md.c                     \
    repo/library/md_wrap.c                \
    repo/library/memory_buffer_alloc.c    \
    repo/library/platform.c               \
    repo/library/sha256.c                 \
    $(NULL)

libmbedcrypto_a_SOURCES                += \
    repo/library/aes.c                    \
    $(NULL)

if OPENVC_BUILD_COVERAGE
Dash                                    = -
CLEANFILES                              = $(shell find $(top_builddir)/third_party/mbedtls $(Dash)name "*.gcda" $(Dash)o $(Dash)name "*.gcno")
endif # OPENVC_BUILD_COVERAGE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
