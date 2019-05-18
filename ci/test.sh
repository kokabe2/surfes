#!/bin/sh

SCRIPT_DIR=$(dirname $(readlink -f $0))

test_package () {
    mkdir -p build
    cd build
    cmake ..
    make
    ctest
}

cd ${SCRIPT_DIR}/../boot/boot && test_package
cd ${SCRIPT_DIR}/../utility && test_package
