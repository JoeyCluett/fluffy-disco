#!/bin/bash

g++ -o mystackvm main.cpp -I./libs -std=c++11 -ffast-math -march=native

INSTALL_LOC="/usr/local/bin/"

if [ -e ${INSTALL_LOC}ystackvm ]
then
    rm ${INSTALL_LOC}mystackvm
fi
cp mystackvm ${INSTALL_LOC}
