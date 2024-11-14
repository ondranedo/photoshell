#!/bin/bash

btype=Debug
build=build/cmake
target=CMakeLists.txt

if [[ "$1" != "" ]]; then
    build=$1/build/cmake
    target=$1/CMakeLists.txt
fi

cmake ${target} -B ${build} -DCMAKE_BUILD_TYPE=${btype}
cmake --build ${build}
