include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    platform                              \
    $(NULL)

SUBDIRS                                 = \
    platform                              \
    $(NULL)

PRETTY_SUBDIRS                          = \
    platform                              \
    $(NULL)

openvc_headers                          = \
    cli.h                                 \
    config.h                              \
    error.h                               \
    instance.h                            \
    tasklet.h                             \
    $(NULL)

openvcdir = $(includedir)/openvc
dist_openvc_HEADERS = $(openvc_headers)

include_HEADERS                         = \
    $(NULL)

install-headers: install-includeHEADERS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
