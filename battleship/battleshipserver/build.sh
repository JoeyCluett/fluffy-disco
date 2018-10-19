#!/bin/bash

STD_OPTS="-std=c++11 -Wall -O3 -march=native"
GL_OPTS="-lSDL -lSDL_gfx -lSDL_ttf"
INC_OPTS="-I../../libs/"

OBJ=( "tcp/Packet" "tcp/Client" )

# generate object code for main.cpp
g++ -g -c -o main.o main.cpp $STD_OPTS $INC_OPTS

LINK_STRING="" # empty string to start

# generate linkage string
for i in "${OBJ[@]}"
do
    LINK_STRING+=" ../../libs/obj/$i.o" # add every object file needed
done

# generate final executable for main.o
g++ -o main main.o $LINK_STRING $STD_OPTS $GL_OPTS