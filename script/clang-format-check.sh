#!/bin/sh

# clang-format does not return a non-zero exit code.  This wrapper
# exits with a non-zero exit code if clang-format outputs any
# replacements.

set -x

# from `man diff`:
# Exit status is 0 if inputs are the same, 1 if different, 2 if trouble.

$(dirname "$0")/clang-format.sh -style=file $@  | diff -u $@ -
