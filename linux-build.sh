#!/bin/bash

btype=Debug
build=build/cmake

if [[ "$1" != "" ]]; then
    build=$1/build/cmake
fi

cmake CMakeLists.txt -B ${build} -DCMAKE_BUILD_TYPE=${btype}
cmake --build ${build}
