#!/bin/sh

SCRIPT_DIR=$(dirname $(readlink -f $0))

test_package () {
    mkdir -p build
    cd build
    cmake ..
    make
    ctest
}

cd ${SCRIPT_DIR}/../developments/boot/boot && test_package
cd ${SCRIPT_DIR}/../developments/boot/registry && test_package
cd ${SCRIPT_DIR}/../developments/core/boot && test_package
cd ${SCRIPT_DIR}/../developments/core/kernel && test_package
cd ${SCRIPT_DIR}/../developments/updater/boot && test_package
cd ${SCRIPT_DIR}/../developments/utility && test_package
