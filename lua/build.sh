g++ -c -o main.o main.cpp -I./../libs/inc/ -std=c++11 -O3 -march=native -llua5.1 -ldl
g++ ./../libs/obj/lua/LuaInterface.o main.o -o main -std=c++11 -O3 -march=native -llua5.1 -ldl
