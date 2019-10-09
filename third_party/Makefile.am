include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    mbedtls                               \
    $(NULL)

if OPENVC_ENABLE_BUILTIN_MBEDTLS
SUBDIRS                                 = \
    mbedtls                               \
    $(NULL)
endif # OPENVC_ENABLE_BUILTIN_MBEDTLS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
