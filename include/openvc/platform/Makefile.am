include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

vc_platform_headers                     = \
    alarm-micro.h                         \
    alarm-milli.h                         \
    debug_uart.h                          \
    logging.h                             \
    misc.h                                \
    random.h                              \
    time.h                                \
    toolchain.h                           \
    uart.h                                \
    $(NULL)

noinst_HEADERS                          = \
    $(NULL)

vc_platformdir = $(includedir)/openvc/platform
dist_vc_platform_HEADERS = $(vc_platform_headers)

install-headers: install-includeHEADERS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
