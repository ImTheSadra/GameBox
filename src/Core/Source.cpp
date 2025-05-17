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
    auto error = luaL_dofile(L, filename);
    if (error != LUA_OK) {
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
    try {
        if (!L) {
            throw MException(__LINE__, __FILE__, "Lua state is null");
        }

        const int initialStack = lua_gettop(L);

        lua_getglobal(L, funcName);

        if (!lua_isfunction(L, -1)) {
            const char* typeName = lua_typename(L, lua_type(L, -1));
            lua_pop(L, 1); 
            throw MException(__LINE__, __FILE__, 
                std::string("'") + funcName + "' is not a function (found " + typeName + ")");
        }

        for (const auto& arg : args) {
            try {
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
                        throw MException(__LINE__, __FILE__, 
                            "Unsupported LuaValue type in function call");
                }
            } catch (const exception& e) {
                lua_settop(L, initialStack);
                throw MException(__LINE__, __FILE__, 
                    std::string("Failed to push argument: ") + e.what());
            }
        }

        int error = lua_pcall(L, args.size(), 0, 0);
        if (error != LUA_OK) {
            const char* errMsg = lua_tostring(L, -1);
            
            lua_Debug debug;
            int gotDebug = 0;
            if (lua_getstack(L, 1, &debug)) {
                gotDebug = lua_getinfo(L, "nSl", &debug);
            }

            MException e(gotDebug ? debug.currentline : __LINE__,
                        gotDebug ? debug.source : __FILE__,
                        errMsg ? errMsg : "Unknown Lua error");
            e.ShowMessageBox();
            lua_pop(L, 1);
            
            throw e;
        }

    } catch (const MException&) {
        throw;
    } catch (const exception& error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw MException(__LINE__, __FILE__, e.what());
    } catch (...) {
        auto e = MException(__LINE__, __FILE__, "Unknown exception in callFunc");
        e.ShowMessageBox();
        throw e;
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
    lua_pop(L, 1);
    return value;
}