#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Source.h"
#include "../MException.hpp"
#include "luaObj.h"

using namespace std;

static Uint8* keyState;

class Engine{
public:
    Engine();
    ~Engine();
    void init();
    void run();

    static int fill(lua_State* L);
    static int line(lua_State* L);
    static int color(lua_State* L);
    static int btn(lua_State* L);
    static int rect(lua_State* L);
private:
    // static Uint8* keyState;
    bool running = true;
    Source source;
};