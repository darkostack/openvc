#!/bin/sh

CLANG_FORMAT_VERSION="clang-format version 5.0"

die() {
    echo " *** ERROR: " $*
    exit 1
}

if which clang-format-5.0 > /dev/null; then
    alias clang-format=clang-format-5.0
elif which clang-format > /dev/null; then
    case "$(clang-format --version)" in
        "$CLANG_FORMAT_VERSION"*)
            ;;
        *)
            die "clang-format 5.0 required"
            ;;
    esac
else
    die "clang-format 5.0 required"
fi

clang-format $@
