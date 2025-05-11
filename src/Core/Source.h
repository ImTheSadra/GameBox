#pragma once

extern "C" {
    #include <lua5.4/lua.h>
    #include <lua5.4/lualib.h>
    #include <lua5.4/lauxlib.h>
}

#include <SDL3/SDL.h>
#include "../Assets/Assets.h"
// #include "../Assets/Shader.h"

static SDL_Window* window;
static SDL_GLContext ctx;
static Assets assets;

class Source {
public:
    Source();
    ~Source();
    
    void init(const char* filename);
    void registerFunc(const char* name, int (*luaFunc)(lua_State*));
    void callFunc(const char* funcName);
    void setInt(const char* name, int val);

private:
    lua_State* L;
};