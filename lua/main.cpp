#include <iostream>

#include <string>
#include <string.h>

#include <lua/LuaInterface.h>

using namespace std;

int main(int argc, char* argv[]) {
    {
        LuaInterface lu;
        lu.runScript("script.lua");

        cout 
            << lu.getGlobal<LI_NUMBER_TYPE>("width") 
            << endl << lu.getGlobal<LI_NUMBER_TYPE>("height")
            << endl << lu.getGlobal<LI_STRING_TYPE>("hw")
            << endl;

    } // destructor should show message

    return 0;
}