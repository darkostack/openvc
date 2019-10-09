include $(abs_top_nlbuild_autotools_dir)/automake/pre.am
include $(top_srcdir)/examples/platforms/Makefile.platform.am

bin_PROGRAMS                                                        = \
    $(NULL)

CPPFLAGS_COMMON                                                    += \
    -I$(top_srcdir)/include                                           \
    -I$(top_srcdir)/src/core                                          \
    -I$(top_srcdir)/examples/platforms                                \
    $(NULL)

LDADD_COMMON                                                       += \
    $(NULL)

LDFLAGS_COMMON                                                     += \
    $(NULL)

LIBTOOLFLAGS_COMMON                                                += \
    $(NULL)

SOURCES_COMMON                                                     += \
    main.c                                                            \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
LDADD_COMMON                                                       += \
    $(top_builddir)/third_party/mbedtls/libmbedcrypto.a               \
    $(NULL)
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

bin_PROGRAMS                                                       += \
    vc-cli                                                            \
    $(NULL)

vc_cli_CPPFLAGS                                                     = \
    $(CPPFLAGS_COMMON)                                                \
    $(NULL)

vc_cli_LDADD                                                        = \
    $(top_builddir)/src/cli/libopenvc-cli.a                           \
    $(top_builddir)/src/core/libopenvc-core.a                         \
    $(LDADD_COMMON)                                                   \
    $(top_builddir)/src/core/libopenvc-core.a                         \
    $(LDADD_COMMON)                                                   \
    $(NULL)

vc_cli_LDFLAGS                                                      = \
    $(LDFLAGS_COMMON)                                                 \
    $(NULL)

vc_cli_LIBTOOLFLAGS                                                 = \
    $(LIBTOOLFLAGS_COMMON)                                            \
    $(NULL)

vc_cli_SOURCES                                                      = \
    $(SOURCES_COMMON)                                                 \
    $(NULL)

if OPENVC_ENABLE_LINKER_MAP
vc_cli_LDFLAGS                                                      += -Wl,-Map=vc-cli.map
endif

if OPENVC_BUILD_COVERAGE
CLEANFILES                                                          = $(wildcard *.gcda *.gcno)
endif # OPENVC_BUILD_COVERAGE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
