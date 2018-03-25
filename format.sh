#!/usr/bin/env bash
FILE_LIST="$(find . -type f -name '*.cpp' -o -name '*.hpp' -o -name '*.cuh' -o -name '*.cu' | grep -v build | grep -v thirdparty)"
clang-format -i -style=file $FILE_LIST
