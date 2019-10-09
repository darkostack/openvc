include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    platforms                             \
    apps                                  \
    $(NULL)

SUBDIRS                                 = \
    platforms                             \
    $(NULL)

if OPENVC_EXAMPLES
SUBDIRS                                += \
    apps                                  \
    $(NULL)
endif

PRETTY_SUBDIRS                          = \
    platforms                             \
    apps                                  \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
