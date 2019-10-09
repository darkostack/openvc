include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                           = libopenvc-platform-utils.a

libopenvc_platform_utils_a_CPPFLAGS     = \
    -I$(top_srcdir)/include               \
    -I$(top_srcdir)/examples/platforms    \
    -I$(top_srcdir)/src/core              \
    $(NULL)

libopenvc_platform_utils_a_SOURCES      = \
    debug_uart.c                          \
    $(NULL)

noinst_HEADERS                          = \
    code_utils.h                          \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
