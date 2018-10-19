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
    echo "    --clean : remove all non-source/header files (.o .bin .exe)"
    echo "    --force : force a rebuild of main, useful when build script changes${RST}"
}

if [ $# -ne 1 ] # we need exactly one argument
then
    echo "${GRN}  Expecting only one argument"
    print_command_line_opts
    exit 128 # invalid argument
fi

# some command line args for g++
STD_OPTS="-std=c++11 -march=native -O3 -Wall -lSDL -lpthread"
INC_OPTS="-I./ -I../libs/inc/" # the default directory for headers in this project
EXT_OBJ="../libs/obj"

if [ $1 == --lib ] # build object code files
then
    echo "${YEL}  Building remote object files..."
    cd ../libs && bash build.sh --lib
    cd ../combat-server/

    echo "${YEL}  Building local object (.o) files for linking..."

    # list of files that need to be compiled into object code
    LINK_FILES=( "TcpServer" )

    for i in "${LINK_FILES[@]}" # iterate through the files
    do
        echo "${CYN}    lib/$i.cpp"
        SRC="lib/$i.cpp"
        LIB="lib/$i.o"

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

    # remove every object code file
    rm -rf lib/*.o
    rm main
    rm main.o

elif [ $1 == --bin ]
then
    
    STAT_SRC=`stat -c %Y main.cpp`
    STAT_OUT=0

    if [ -e "main.o" ]
    then
        STAT_OUT=`stat -c %Y main.o`
    fi

    if [ $STAT_SRC -gt $STAT_OUT ]
    then
        echo "     -- main.cpp newer, recompiling..."
        g++ -c -o main.o main.cpp $STD_OPTS $INC_OPTS

        # link final executable as well
        g++ main.o ./lib/TcpServer.o \
        $EXT_OBJ/tcp/Packet.o $EXT_OBJ/tcp/Client.o \
        -o main $STD_OPTS $INC_OPTS 
    else
        echo "     -- main.o newer, skipping compilation..."
    fi

elif [ $1 == --force ]
then
    echo "     forcing recompilation of main..."
    rm main
    rm main.o
    bash build.sh --bin
fi 
