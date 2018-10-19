#include <lua/LuaInterface.h>

#include <stdexcept>
#include <iostream>

const char* INTERNAL_LUA_TO_STRING(lua_State* s, int i) {
    return lua_tostring(s, i);
}

LuaInterface::LuaInterface(void) {
    this->lua_state = lua_open();
    luaL_openlibs(this->lua_state);
}

LuaInterface::~LuaInterface(void) {
    std::cout << "LuaInterface::~LuaInterface -> releasing Lua resources\n" << std::flush;
    lua_close(this->lua_state);
}

void LuaInterface::runScript(std::string filename) {
    if(luaL_loadfile(lua_state, filename.c_str()) || lua_pcall(lua_state, 0, 0, 0)) {
        std::string err_msg = "LuaInterface::runScript() error -> ";
        err_msg += lua_tostring(lua_state, -1);
        throw std::runtime_error(err_msg);
    }
}

