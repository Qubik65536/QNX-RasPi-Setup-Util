#!/bin/bash
 
source /usr/local/qnx/env/bin/activate
source $HOME/qnx800/qnxsdp-env.sh
 
cd $HOME/qnxprojects/qnx-raspi-setup-util/
 
# ntoaarch64-gcc -o my-project -Igpio/aarch64/ my-project.c
CC=ntoaarch64-gcc
CXX=ntoaarch64-g++
cmake -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX .
cmake --build .
