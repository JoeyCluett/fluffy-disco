#!/bin/bash

# pretty text awaits
RED=`tput setaf 1` # red
GRN=`tput setaf 2` # green
YEL=`tput setaf 3` # yellow
MAG=`tput setaf 5` # magenta
CYN=`tput setaf 6` # cyan
RST=`tput sgr0`    # default

print_command_line_opts()
{
    echo "${RED}  Options:"
    echo "    --lib   : (re)build all of the .o files needed for linking libraries"
    echo "    --bin   : (re)build all of the c++ executables (test files and final executable)"
    echo "    --clean : remove all non-source/header files (.o .bin .exe)${RST}"
}

if [ $# -ne 1 ] # we need exactly one argument
then
    echo "${GRN}  Expecting only one argument"
    print_command_line_opts
    exit 128 # invalid argument
fi

# some command line args for g++
STD_OPTS="-std=c++11 -march=native -O3 -Wall -lSDL"
INC_OPTS="-I./inc/ -I./" # the default directory for headers in this project

if [ $1 == --lib ] # build object code files
then
    echo "${YEL}  Building object (.o) files for linking..."

    # list of files that need to be compiled into object code
    LINK_FILES=( "Animation" "FloatRect" "HitBox" "MineExplosion" \
    "Polygon" "Projectile" "TextTag" "LevelLoader" "FloatLine" )

    # libraries needed to connect to combat-clone server
    LINK_FILES+=( "tcp/Packet" "tcp/Client" )

    # libraries for lua interfacing
    LINK_FILES+=( "lua/LuaInterface" )

    for i in "${LINK_FILES[@]}" # iterate through the files
    do
        echo "${CYN}    src/$i.cpp"
        SRC="src/$i.cpp"
        LIB="obj/$i.o"

        STAT_SRC=`stat -c %Y $SRC`
        STAT_OUT=0

        if [ -e $LIB ] # make sure the .o file exists before testing it with stat
        then
            STAT_OUT=`stat -c %Y $LIB`
        fi

        if [ $STAT_SRC -gt $STAT_OUT ]
        then
            echo "      -- $SRC newer, recompiling..."
            g++ -c -o $LIB $SRC $STD_OPTS $INC_OPTS
        else
            echo "      -- $LIB newer, skipping compilation..."
        fi

    done

elif [ $1 == --clean ] # delete all unneccessary files
then
    echo "${YEL}  Cleaning up the workspace..."

    # remove everything in bin/ and obj/
    rm -rf obj/*.o
    rm -rf obj/tcp/*.o
    rm -rf obj/lua/*.o

fi 

# i never finish anythi

