#!/bin/sh

die() {
    echo " *** ERROR: " $*
    exit 1
}

set -x

cd /tmp || die

[ $TRAVIS_OS_NAME != linux ] || {
    sudo apt-get update || die
    sudo apt-get install lcov || die

    [ $BUILD_TARGET != posix-32-bit ] || {
        pip install --user --upgrade pip || die
    }

    [ $BUILD_TARGET != pretty-check ] || {
        clang-format --version || die
    }

    [ $BUILD_TARGET != scan-build ] || {
        sudo apt-get install clang || die
    }

    [ $BUILD_TARGET != posix-32-bit ] || {
        sudo apt-get install g++-multilib || die
    }

    [ $BUILD_TARGET != posix -o $CC != clang ] || {
        sudo apt-get install clang || die
    }
}

[ $TRAVIS_OS_NAME != osx ] || {
    sudo easy_install pexpect || die
}
