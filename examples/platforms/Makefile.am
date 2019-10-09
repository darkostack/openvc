include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    posix                                 \
    $(NULL)

SUBDIRS                                 = \
    utils                                 \
    $(NULL)

if OPENVC_EXAMPLES_POSIX
SUBDIRS                                += posix
endif

noinst_HEADERS                          = \
    openvc-system.h                       \
    $(NULL)

PRETTY_SUBDIRS                          = \
    posix                                 \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
