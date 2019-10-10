include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                           = \
    libopenvc-cli.a                       \
    $(NULL)

CPPFLAGS_COMMON                         = \
    -I$(top_srcdir)/include               \
    -I$(top_srcdir)/src                   \
    -I$(top_srcdir)/src/core              \
    $(OPENVC_TARGET_DEFINES)              \
    $(NULL)

if OPENVC_POSIX
CPPFLAGS_COMMON                        += \
    -DOPENVC_POSIX=1                      \
    $(NULL)
endif

libopenvc_cli_a_CPPFLAGS                = \
    $(CPPFLAGS_COMMON)                    \
    $(NULL)

SOURCES_COMMON                          = \
    cli.cpp                               \
    cli_server.cpp                        \
    cli_uart.cpp                          \
    $(NULL)

libopenvc_cli_a_SOURCES                 = \
    $(SOURCES_COMMON)                     \
    $(NULL)

noinst_HEADERS                          = \
    cli.hpp                               \
    cli_config.h                          \
    cli_server.hpp                        \
    cli_uart.hpp                          \
    $(NULL)

if OPENVC_BUILD_COVERAGE
CLEANFILES                              = $(wildcard *.gcda *.gcno)
endif # OPENVC_BUILD_COVERAGE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
