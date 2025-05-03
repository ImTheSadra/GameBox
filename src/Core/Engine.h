#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "Source.h"
#include "../MException.hpp"
#include "luaObj.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

static Uint8* keyState;

class Engine{
public:
    Engine();
    ~Engine();
    void init(const char* path);
    void run();

    static int fill(lua_State* L);
    static int vertex3(lua_State* L);
    static int color(lua_State* L);
    static int beginQuad(lua_State* L);
    static int endDraw(lua_State* L);
    static int btn(lua_State* L);
    static int rotate(lua_State* L);
    static int loadTexture(lua_State* L);
    static int useTexture(lua_State* L);
    static int texCoord(lua_State* L);
private:
    static void handleError(int line, string file);
    // static Uint8* keyState;
    bool running = true;
    Source source;
};