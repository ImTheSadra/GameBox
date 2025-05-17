#pragma once

extern "C" {
    #include <lua5.4/lua.h>
    #include <lua5.4/lualib.h>
    #include <lua5.4/lauxlib.h>
}

#include <SDL3/SDL.h>
#include <vector>
#include "LuaValue.h"
#include <unordered_map>
#include "../Assets/Assets.h"
#include "../Assets/Shader.h"

using namespace std;

static SDL_Window* window;
static SDL_GLContext ctx;
static Assets assets;
static Shader* shader;
static unordered_map<SDL_JoystickID, SDL_Joystick*> Joysticks = {};

class Source {
public:
    Source();
    ~Source();
    
    void init(const char* filename);
    void registerFunc(const char* name, int (*luaFunc)(lua_State*));
    void callFunc(const char* funcName, const vector<LuaValue> &arguments = {});
    void setInt(const char* name, int val);
    bool functionExists(const char* funcName);
    bool readBool(const char* name);
private:
    lua_State* L;
};