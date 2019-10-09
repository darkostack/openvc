include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

DIST_SUBDIRS                            = \
    core                                  \
    cli                                   \
    $(NULL)

SUBDIRS                                 = \
    core                                  \
    $(NULL)

if OPENVC_ENABLE_CLI
SUBDIRS                                += cli
endif

PRETTY_SUBDIRS                          = \
    core                                  \
    cli                                   \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
