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

void Source::callFunc(const char* funcName, const vector<LuaValue>& args) {
    lua_getglobal(L, funcName);

    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1); 
        throw runtime_error(string(funcName) + " is not a function!");
    }

    for (const auto& arg : args) {
        switch (arg.type) {
            case LuaValue::Type::NIL:
                lua_pushnil(L);
                break;
            case LuaValue::Type::NUMBER:
                lua_pushnumber(L, arg.getNumber());
                break;
            case LuaValue::Type::STRING:
                lua_pushstring(L, arg.getString().c_str());
                break;
            case LuaValue::Type::BOOLEAN:
                lua_pushboolean(L, arg.getBoolean());
                break;
            case LuaValue::Type::INTEGER:
                lua_pushinteger(L, arg.getInt());
                break;
            default:
                throw runtime_error("Unsupported LuaValue type");
        }
    }

    if (lua_pcall(L, args.size(), 0, 0) != LUA_OK) {
        const char* error = lua_tostring(L, -1);
        lua_pop(L, 1);
        throw runtime_error(string("Lua error: ") + error);
    }
}

void Source::setInt(const char* name, int val){
    lua_pushinteger(L, val);
    lua_setglobal(L, name);
}

bool Source::functionExists(const char* funcName) {
    lua_getglobal(L, funcName);
    bool isFunction = lua_isfunction(L, -1); 
    lua_pop(L, 1);             
    return isFunction;
}

bool Source::readBool(const char* name){
    lua_getglobal(L, name);
    bool value = luaL_checkinteger(L, 1);
    // luaL_checkany()
    lua_pop(L, 1);
    return value;
}