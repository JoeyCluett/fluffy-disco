#ifndef __JJC__LUA__INTERFACE__H__
#define __JJC__LUA__INTERFACE__H__

#include <string>
#include <stdexcept>

extern "C" {
    #include <lua5.1/lua.h>
    #include <lua5.1/lauxlib.h>
    #include <lua5.1/lualib.h>
}

// convenience defines for the getGlobal method
#define LI_NUMBER_TYPE   double,LUA_TNUMBER,lua_tonumber
#define LI_BOOLEAN_TYPE  int,LUA_TBOOLEAN,lua_toboolean
#define LI_STRING_TYPE   const char*,LUA_TSTRING,INTERNAL_LUA_TO_STRING

// ugly hack to get lua_tostring working properly
const char* INTERNAL_LUA_TO_STRING(lua_State*, int);

class LuaInterface {
private:
    lua_State* lua_state;

public:
    // load all the Lua libraries needed
    LuaInterface(void);

    // release memory associated with the 
    // currently open lua_State
    ~LuaInterface(void);

    // run an external Lua script in current Lua context
    void runScript(std::string filename);

    // templates must be locally defined in C++
    template<typename T, int __LUA_TYPE, T(*callback)(lua_State*, int)>
    T getGlobal(std::string name) {
        lua_getglobal(lua_state, name.c_str());
        if(lua_type(lua_state, -1) == __LUA_TYPE) {
            return callback(lua_state, -1);
        } else {
            throw std::runtime_error("LuaInterface::getGlobal -> requested type does not match retrieved value");
        }
    }

};

#endif // __JJC__LUA__INTERFACE__H__