#!/bin/sh

die() {
    echo " *** ERROR: " $*
    exit 1
}

set -x

[ $BUILD_TARGET != pretty-check ] || {
    ./bootstrap || die
    ./configure || die
    make pretty-check || die
}

[ $BUILD_TARGET != scan-build ] || {
    ./bootstrap || die

    scan-build ./configure              \
        --enable-cli-app=yes            \
        --with-examples=posix || die
    scan-build --status-bugs -analyze-headers -v make || die
}

[ $BUILD_TARGET != posix ] || {
    sh -c '$CC --version' || die
    sh -c '$CXX --version' || die

    git checkout -- . || die
    git clean -xfd || die
    ./bootstrap || die
    CPPFLAGS=-DCONFIG_LOG_LEVEL=LOG_LEVEL_NONE make -f examples/Makefile-posix check || die

    git checkout -- . || die
    git clean -xfd || die
    ./bootstrap || die
    CPPFLAGS=-DCONFIG_LOG_LEVEL=LOG_LEVEL_DEBG make -f examples/Makefile-posix check || die
}

[ $BUILD_TARGET != posix-32-bit ] || {
    ./bootstrap || die

    ./configure                         \
        --enable-cli-app=yes            \
        --with-examples=posix           \
        --enable-build-coverage=yes || die

    COVERAGE=1 CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32 make check || die
}
