include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                           = libopenvc-posix.a

libopenvc_posix_a_CPPFLAGS              = \
    -I$(top_srcdir)/include               \
    -I$(top_srcdir)/examples/platforms    \
    -I$(top_srcdir)/src/core              \
    -D_GNU_SOURCE                         \
    $(NULL)

PLATFORM_SOURCES                        = \
    alarm.c                               \
    logging.c                             \
    misc.c                                \
    random.c                              \
    system.c                              \
    uart-posix.c                          \
    $(NULL)

libopenvc_posix_a_SOURCES               = \
    $(PLATFORM_SOURCES)                   \
    $(NULL)

noinst_HEADERS                          = \
    platform-posix.h                      \
    $(NULL)

PRETTY_FILES                            = \
    $(PLATFORM_SOURCES)                   \
    $(noinst_HEADERS)                     \
    $(NULL)

Dash                                    = -
libopenvc_posix_a_LIBADD                = $(shell find $(top_builddir)/examples/platforms/utils $(Dash)type f $(Dash)name "*.o")

if OPENVC_BUILD_COVERAGE
CLEANFILES                              = $(wildcard *.gcda *.gcno)
endif # OPENVC_BUILD_COVERAGE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
