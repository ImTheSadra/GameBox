#include "Source.h"
#include "../MException.hpp"
#include <iostream>

using namespace std;

//Source class

Source::Source() {
    L = luaL_newstate();
    if (!L) {
        auto e = MException(__LINE__, __FILE__, "Failed to create Lua state!");
        e.ShowMessageBox();
        throw e;
    }
    luaL_openlibs(L);
}

Source::~Source() {
    if (L) {
        lua_close(L);
    }
}

void Source::init(const char* filename) {
    if (luaL_dofile(L, filename) != LUA_OK) {
        auto e = MException(__LINE__, __FILE__, "Error loading Lua file");
        e.ShowMessageBox();
        lua_pop(L, 1);
        throw e;
    }
}

void Source::registerFunc(const char* name, int (*luaFunc)(lua_State*)) {
    lua_register(L, name, luaFunc);
}

void Source::callFunc(const char* funcName) {
    lua_getglobal(L, funcName);

    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            auto e = MException(__LINE__, __FILE__, "Error calling Lua function " + (string)funcName + lua_tostring(L, -1));
            lua_pop(L, 1); 
            e.ShowMessageBox();
            throw e;
        }
    } else {
        auto e = MException(__LINE__, __FILE__, (string)funcName + " is not a function!");
        lua_pop(L, 1); 
        e.ShowMessageBox();
        throw e;
    }
}

void Source::setInt(const char* name, int val){
    lua_pushinteger(L, val);
    lua_setglobal(L, name);
}