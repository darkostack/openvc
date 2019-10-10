#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#
# Declare autoconf version requirements
#
AC_PREREQ([2.68])

#
# Initialize autoconf for the package
#
AC_INIT([OPENVC],
        m4_esyscmd([third_party/nlbuild-autotools/repo/scripts/mkversion -b `cat .default-version` .]),
        [darkopancev.dev@gmail.com],
        [openvc],
        [http://github.com/darkostack/openvc])

# Tell the rest of the build system the absolute path where the
# nlbuild-autotools repository is rooted at.

AC_SUBST(nlbuild_autotools_stem,[third_party/nlbuild-autotools/repo])
AC_SUBST(abs_top_nlbuild_autotools_dir,[\${abs_top_srcdir}/\${nlbuild_autotools_stem}])

#
# OPENVC interface current, revision, and age versions.
#
# Maintainters: Please manage these fields as follows:
#
#   Interfaces removed:    CURRENT++, AGE = 0, REVISION = 0
#   Interfaces added:      CURRENT++, AGE++,   REVISION = 0
#   No interfaces changed:                     REVISION++
#
#
AC_SUBST(LIBOPENVC_VERSION_CURRENT,  [0])
AC_SUBST(LIBOPENVC_VERSION_AGE,      [5])
AC_SUBST(LIBOPENVC_VERSION_REVISION, [0])
AC_SUBST(LIBOPENVC_VERSION_INFO,     [${LIBOPENVC_VERSION_CURRENT}:${LIBOPENVC_VERSION_REVISION}:${LIBOPENVC_VERSION_AGE}])

#
# Check the sanity of the source directory by checking for the
# presence of a key watch file
#
AC_CONFIG_SRCDIR([include/openvc/error.h])

#
# Tell autoconf where to find auxilliary build tools (e.g. config.guess,
# install-sh, missing, etc.)
#
AC_CONFIG_AUX_DIR([third_party/nlbuild-autotools/repo/third_party/autoconf])

#
# Tell autoconf where to find auxilliary M4 macros
#
AC_CONFIG_MACRO_DIRS([third_party/nlbuild-autotools/repo/third_party/autoconf/m4 third_party/nlbuild-autotools/repo/autoconf/m4])

#
# Tell autoconf what file the package is using to aggregate C preprocessor
# defines.
#
AC_CONFIG_HEADERS([include/openvc-config-generic.h])

#
# Figure out what the canonical build and host tuples are.
#
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

#
# Mac OS X / Darwin ends up putting some versioning cruft on the end of its
# tuple that we don't care about in this script. Create "clean" variables
# devoid of it.
#

NL_FILTERED_CANONICAL_BUILD
NL_FILTERED_CANONICAL_HOST

#
# Configure automake with the desired options, indicating that this is not
# a native GNU package, that we want "silent" build rules, and that we want
# objects built in the same subdirectory as their source rather than collapsed
# together at the top-level directory.
#
# Disable silent build rules by either passing --disable-silent-rules to
# configure or passing V=1 to make
#
AM_INIT_AUTOMAKE([1.14 foreign silent-rules subdir-objects tar-pax])

#
# Silent build rules requires at least automake-1.11. Employ
# techniques for not breaking earlier versions of automake.
#
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_SILENT_RULES([yes])

#
# Enable maintainer mode to prevent the package from constantly trying
# to rebuild configure, Makefile.in, etc. Rebuilding such files rarely,
# if ever, needs to be done "in the field".
#
# Use the included 'bootstrap' script instead when necessary.
#
AM_MAINTAINER_MODE

#
# Host-os-specific checks
#

case ${host_os} in

    *darwin*)
        OPENVC_TARGET=darwin
        OPENVC_TARGET_DEFINES="-DOPENVC_TARGET_DARWIN"
        ;;

    *linux*)
        OPENVC_TARGET=linux
        OPENVC_TARGET_DEFINES="-DOPENVC_TARGET_LINUX"
        ;;

esac

AC_SUBST(OPENVC_TARGET_DARWIN)
AM_CONDITIONAL([OPENVC_TARGET_DARWIN], [test "${OPENVC_TARGET}" = "darwin"])

AC_SUBST(OPENVC_TARGET_LINUX)
AM_CONDITIONAL([OPENVC_TARGET_LINUX], [test "${OPENVC_TARGET}" = "linux"])

AC_SUBST(OPENVC_TARGET_DEFINES)

#
# Checks for build host programs
#

# This is a hack to restore some old broken behavior that was
# removed in pull request #1527. It's use is highly discouraged,
# you should try to fix your build environment instead.
AC_ARG_ENABLE(no-executables-hack,
    [AS_HELP_STRING([--enable-no-executables-hack],
        [Enable hack that prevents link checks at configure time. Highly discouraged.])])
AC_MSG_CHECKING([whether to disable executable checking])
if test "${enable_no_executables_hack}" = "yes"
then
    AC_MSG_RESULT([yes])
    AC_NO_EXECUTABLES
    # Here we guess conservative values for tests that require link checks
    # to test for these features. This will prevent these checks from
    # being performed later in the configuration process.
    ac_cv_func_strlcat=${ac_cv_func_strlcat-no}
    ac_cv_func_strlcpy=${ac_cv_func_strlcpy-no}
    ac_cv_func_strnlen=${ac_cv_func_strnlen-no}
else
    AC_MSG_RESULT([no])
fi

# Passing -Werror to GCC-based or -compatible compilers breaks some
# autoconf tests (see
# http://lists.gnu.org/archive/html/autoconf-patches/2008-09/msg00014.html).
#
# If -Werror has been passed transform it into -Wno-error. We'll
# transform it back later with NL_RESTORE_WERROR.

NL_SAVE_WERROR

# Check for compilers.
#
# These should be checked BEFORE we check for and, implicitly,
# initialize libtool such that libtool knows what languages it has to
# work with.

AC_PROG_CPP
AC_PROG_CPP_WERROR

AC_PROG_CC
AC_PROG_CC_C_O

AC_PROG_CXXCPP

AC_PROG_CXX
AC_PROG_CXX_C_O

AM_PROG_AS

# Check for other compiler toolchain tools.

AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib)
AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(STRIP, strip)

# Check for other host tools.

AC_PROG_INSTALL
AC_PROG_LN_S

AC_PATH_PROG(CMP, cmp)

#
# Checks for specific compiler characteristics
#

#
# Common compiler flags we would like to have.
#
#   -Wall                        CC, CXX
#

PROSPECTIVE_CFLAGS="-Wall -Wextra -Wshadow -Werror -std=c99 -pedantic-errors -Wno-gnu-zero-variadic-macro-arguments"
PROSPECTIVE_CXXFLAGS="-Wall -Wextra -Wshadow -Werror -std=gnu++98 -Wno-c++14-compat -fno-exceptions"

AC_CACHE_CHECK([whether $CC is Clang],
    [nl_cv_clang],
    [nl_cv_clang=no
    if test "x${GCC}" = "xyes"; then
        AC_EGREP_CPP([NL_CC_IS_CLANG],
            [/* Note: Clang 2.7 lacks __clang_[a-z]+__ */
#            if defined(__clang__) && defined(__llvm__)
             NL_CC_IS_CLANG
#            endif
            ],
            [nl_cv_clang=yes])
    fi
    ])

if test "${nl_cv_clang}" = "yes"; then
    PROSPECTIVE_CFLAGS="${PROSPECTIVE_CFLAGS} -Wconversion"
    PROSPECTIVE_CXXFLAGS="${PROSPECTIVE_CXXFLAGS} -Wconversion"
fi

AX_CHECK_COMPILER_OPTIONS([C],   ${PROSPECTIVE_CFLAGS})
AX_CHECK_COMPILER_OPTIONS([C++], ${PROSPECTIVE_CXXFLAGS})

# Check for and initialize libtool

LT_INIT
AC_PROG_LIBTOOL

# Disable building shared libraries by default (can be enabled with --enable-shared)

AC_DISABLE_SHARED

#
# Debug instances
#
AC_MSG_NOTICE([checking whether to build debug instances])

# Debug

NL_ENABLE_DEBUG([no])

AM_CONDITIONAL([OPENVC_BUILD_DEBUG], [test "${nl_cv_build_debug}" = "yes"])

#
# Code coverage and compiler optimization
#

# Coverage

AC_ARG_ENABLE(build-coverage,
    [AS_HELP_STRING([--enable-build-coverage],[Enable build coverage @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_build_coverage=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_build_coverage} for --enable-build-coverage])
            ;;
        esac
    ],
    [enable_build_coverage=no])

if test ! "${OPENVC_TARGET}" = "darwin" -a "${enable_build_coverage}" = "yes"; then
NL_ENABLE_COVERAGE([yes])
NL_ENABLE_COVERAGE_REPORTS([yes])
else
NL_ENABLE_COVERAGE([no])
NL_ENABLE_COVERAGE_REPORTS([no])
fi

AM_CONDITIONAL([OPENVC_BUILD_COVERAGE], [test "${nl_cv_build_coverage}" = "yes"])
AM_CONDITIONAL([OPENVC_BUILD_COVERAGE_REPORTS], [test "${nl_cv_build_coverage_reports}" = "yes"])

# Optimization

NL_ENABLE_OPTIMIZATION([yes])

AM_CONDITIONAL([OPENVC_BUILD_OPTIMIZED], [test "${nl_cv_build_optimized}" = "yes"])

#
# Code style
#

AC_SUBST(PRETTY, ["\${abs_top_srcdir}/script/clang-format.sh"])
AC_SUBST(PRETTY_ARGS, ["-style=file -i"])
AC_SUBST(PRETTY_CHECK, ["\${abs_top_srcdir}/script/clang-format-check.sh"])
AC_SUBST(PRETTY_CHECK_ARGS, [""])

#
# Tests
#
AC_MSG_NOTICE([checking whether to build tests])

# Tests

NL_ENABLE_TESTS([yes])

AM_CONDITIONAL([OPENVC_BUILD_TESTS], [test "${nl_cv_build_tests}" = "yes"])

#
# Builtin mbedtls
#

AC_ARG_ENABLE(builtin-mbedtls,
    [AS_HELP_STRING([--enable-builtin-mbedtls],[Enable builtin mbedtls @<:@default=yes@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_builtin_mbedtls=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_builtin_mbedtls} for --enable-builtin-mbedtls])
            ;;
        esac
    ],
    [enable_builtin_mbedtls=yes])

if test "$enable_builtin_mbedtls" = "yes" -a ! "${MBEDTLS_CPPFLAGS}"; then
    MBEDTLS_CPPFLAGS="-I\${abs_top_srcdir}/third_party/mbedtls"
    MBEDTLS_CPPFLAGS="${MBEDTLS_CPPFLAGS} -I\${abs_top_srcdir}/third_party/mbedtls/repo/include"
    MBEDTLS_CPPFLAGS="${MBEDTLS_CPPFLAGS} -I\${abs_top_srcdir}/third_party/mbedtls/repo/include/mbedtls"
    MBEDTLS_CPPFLAGS="${MBEDTLS_CPPFLAGS} -DMBEDTLS_CONFIG_FILE=\\\"mbedtls-config.h\\\""
fi
AC_MSG_CHECKING([whether mbed TLS should be enabled])
AC_MSG_RESULT(${enable_builtin_mbedtls})
AM_CONDITIONAL([OPENVC_ENABLE_BUILTIN_MBEDTLS], [test "${enable_builtin_mbedtls}" = "yes"])

#
# Linker Map Output
#

AC_ARG_ENABLE(linker-map,
    [AS_HELP_STRING([--enable-linker-map],[Enable linker map output @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_linker_map=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enable_linker_map} for --enable-linker-map])
            ;;
        esac
    ],
    [enable_linker_map=no])

AC_MSG_CHECKING([whether to enable linker map output])
AC_MSG_RESULT(${enable_linker_map})
AM_CONDITIONAL([OPENVC_ENABLE_LINKER_MAP], [test "${enable_linker_map}" = "yes"])

#
# Examples
#

AC_ARG_WITH(examples,
    [AS_HELP_STRING([--with-examples=TARGET],
        [Specify the examples from one of: none, posix @<:@default=none@:>@.])],
    [
        case "${with_examples}" in
        none)
            ;;
        posix)
            ;;
        *)
            AC_MSG_ERROR([Invalid value ${with_examples} for --with-examples])
            ;;
        esac
    ],
    [with_examples=none])

OPENVC_EXAMPLES=${with_examples}

case ${with_examples} in

    posix)
        OPENVC_EXAMPLES_POSIX=1
        AC_DEFINE_UNQUOTED([OPENVC_EXAMPLES_POSIX],[${OPENVC_EXAMPLES_POSIX}],[Define to 1 if you want to use posix examples])
        ;;

esac

AC_MSG_CHECKING([whether to enable examples])
AC_MSG_RESULT(${OPENVC_EXAMPLES})

AC_SUBST(OPENVC_EXAMPLES)
AM_CONDITIONAL([OPENVC_EXAMPLES], [test ${with_examples} != "none"])
AC_DEFINE_UNQUOTED([OPENVC_EXAMPLES],[${OPENVC_EXAMPLES}],[Openvc examples])

AC_SUBST(OPENVC_EXAMPLES_POSIX)
AM_CONDITIONAL([OPENVC_EXAMPLES_POSIX], [test "${OPENVC_EXAMPLES}" = "posix"])

AM_CONDITIONAL(OPENVC_POSIX, [test "${OPENVC_ENABLE_POSIX_APP}" = "1" -o "${OPENVC_EXAMPLES_POSIX}" = "1"])


#
# CLI Library
#
AC_ARG_ENABLE(cli-app,
    [AS_HELP_STRING([--enable-cli-app],[Enable CLI support (no|yes) @<:@default=no@:>@.])],
    [
        case "${enableval}" in
        no|yes)
            enable_cli_app=${enableval}
            ;;
        *)
            AC_MSG_ERROR([Invalid value ${enableval} for --enable-cli-app])
            ;;
        esac
    ],
    [enable_cli_app=no])

AC_MSG_CHECKING([cli-app modes])
AC_MSG_RESULT(${enable_cli_app})
AM_CONDITIONAL([OPENVC_ENABLE_CLI], [test "${enable_cli_app}" == "yes"])
AC_SUBST(OPENVC_ENABLE_CLI)

#
# Checks for libraries and packages.
#
# At minimum, the following packages are optional, depending on
# configuration:
#
#   * TBD
#
AC_MSG_NOTICE([checking required package dependencies])

# NL_WITH_PACKAGE(...)

#
# Check for headers
#

#---------------------------------------------------
# Enable BSD Security Features
# This enables strlcpy() and other friends in GNU land.
# While the references below generally speak of: "glibc"
# The ARM Embedded platform uses the nano instance of NEWLIB
# Which greatly follows and mirrors glibc.
# --------------------------------------------------
#
# References:
# 1) http://stackoverflow.com/questions/29201515/what-does-d-default-source-do
# 2) http://man7.org/linux/man-pages/man7/feature_test_macros.7.html
#
CFLAGS="${CFLAGS} -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1"
CXXFLAGS="${CXXFLAGS} -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1"

OLD_CFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -Wno-error=address"
AC_HEADER_STDBOOL
CFLAGS="${OLD_CFLAGS}"
AC_HEADER_STDC

AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([string.h])

#
# Missing Functions
#
AC_CHECK_FUNC([strlcpy], [AC_DEFINE([HAVE_STRLCPY], [1], [Define if strlcpy exists.])])
AC_CHECK_FUNC([strlcat], [AC_DEFINE([HAVE_STRLCAT], [1], [Define if strlcat exists.])])
AC_CHECK_FUNC([strnlen], [AC_DEFINE([HAVE_STRNLEN], [1], [Define if strnlen exists.])])

#
# Check for types and structures
#
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

#
# Checks for library functions
#

if test "${ac_no_link}" != "yes"; then
    AC_CHECK_FUNCS([memcpy])
fi

# Add any mbedtls CPPFLAGS

CPPFLAGS="${CPPFLAGS} ${MBEDTLS_CPPFLAGS}"

# Add any code coverage CPPFLAGS and LDFLAGS

CPPFLAGS="${CPPFLAGS} ${NL_COVERAGE_CPPFLAGS}"
LIBS="${LIBS} ${NL_COVERAGE_LIBS}"

# At this point, we can restore the compiler flags to whatever the
# user passed in, now that we're clear of an -Werror issues by
# transforming -Wno-error back to -Werror.

NL_RESTORE_WERROR

#
# Identify the various makefiles and auto-generated files for the package
#
AC_CONFIG_FILES([
Makefile
include/Makefile
include/openvc/Makefile
include/openvc/platform/Makefile
src/Makefile
src/cli/Makefile
src/core/Makefile
third_party/Makefile
third_party/mbedtls/Makefile
examples/Makefile
examples/apps/Makefile
examples/apps/cli/Makefile
examples/platforms/Makefile
examples/platforms/posix/Makefile
examples/platforms/utils/Makefile
tests/Makefile
tests/unit/Makefile
])

#
# Generate the auto-generated files for the package
#
AC_OUTPUT

#
# Summarize the package configuration
#

AC_MSG_NOTICE([

  Configuration Summary
  ---------------------
  Package                                   : ${PACKAGE_NAME}
  Version                                   : ${PACKAGE_VERSION}
  Interface                                 : ${LIBOPENVC_VERSION_INFO//:/.}
  Build system                              : ${build}
  Host system                               : ${host}
  Host architecture                         : ${host_cpu}
  Host OS                                   : ${host_os}
  Cross compiling                           : ${cross_compiling}
  Build shared libraries                    : ${enable_shared}
  Build static libraries                    : ${enable_static}
  Build debug libraries                     : ${nl_cv_build_debug}
  Build optimized libraries                 : ${nl_cv_build_optimized}
  Build coverage libraries                  : ${nl_cv_build_coverage}
  Build coverage reports                    : ${nl_cv_build_coverage_reports}
  Address sanitizer support                 : ${enable_address_sanitizer}
  Lcov                                      : ${LCOV:--}
  Genhtml                                   : ${GENHTML:--}
  Build tests                               : ${nl_cv_build_tests}
  Build fuzz targets                        : ${enable_fuzz_targets}
  Build tools                               : ${build_tools}
  Openvc tests                              : ${with_tests}
  Prefix                                    : ${prefix}
  Documentation support                     : ${nl_cv_build_docs}
  Doxygen                                   : ${DOXYGEN:--}
  GraphViz dot                              : ${DOT:--}
  C Preprocessor                            : ${CPP}
  C Compiler                                : ${CC}
  C++ Preprocessor                          : ${CXXCPP}
  C++ Compiler                              : ${CXX}
  Assembler Compiler                        : ${CCAS}
  Archiver                                  : ${AR}
  Archive Indexer                           : ${RANLIB}
  Symbol Stripper                           : ${STRIP}
  Object Copier                             : ${OBJCOPY}
  C Preprocessor flags                      : ${CPPFLAGS:--}
  C Compile flags                           : ${CFLAGS:--}
  C++ Compile flags                         : ${CXXFLAGS:--}
  Assembler flags                           : ${CCASFLAGS:--}
  Link flags                                : ${LDFLAGS:--}
  Link libraries                            : ${LIBS}
  Link maps                                 : ${enable_linker_map}
  Pretty                                    : ${PRETTY:--}
  Pretty args                               : ${PRETTY_ARGS:--}
  Pretty check                              : ${PRETTY_CHECK:--}
  Pretty check args                         : ${PRETTY_CHECK_ARGS:--}
  Openvc builtin mbedtls support            : ${enable_builtin_mbedtls}
  Openvc CLI support                        : ${enable_cli_app}
  Openvc examples                           : ${OPENVC_EXAMPLES}

])
