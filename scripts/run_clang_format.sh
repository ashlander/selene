#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPO_DIR=${SCRIPT_DIR}/..

find ${REPO_DIR}/selene/  -type f -iname *.hpp -o -iname *.cpp -print0 | xargs -0 clang-format -style=file -i
find ${REPO_DIR}/test/ -type f -iname *.hpp -o -iname *.cpp -print0 | xargs -0 clang-format -style=file -i
