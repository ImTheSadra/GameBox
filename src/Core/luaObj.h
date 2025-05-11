#pragma once

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <unordered_map>

using namespace std;

static unordered_map<char*, int> lua_keys{
    {"K_SPACE", SDLK_SPACE},
    {"K_TAB", SDLK_TAB},
    {"K_UP", SDLK_UP},
    {"K_DOWN", SDLK_DOWN},
    {"K_RIGHT", SDLK_RIGHT},
    {"K_LEFT", SDLK_LEFT},
    {"K_LSHIFT", SDLK_LSHIFT},
    {"K_RSHIFT", SDLK_RSHIFT},
    {"K_LCTRL", SDLK_LCTRL},
    {"K_RCTRL", SDLK_RCTRL},

    {"K_W", SDLK_W},    {"K_D", SDLK_D},
    {"K_A", SDLK_A},    {"K_S", SDLK_S},
    {"K_Q", SDLK_Q},    {"K_E", SDLK_E},
    {"K_R", SDLK_R},    {"K_T", SDLK_T},
    {"K_U", SDLK_U},    {"K_I", SDLK_I},
    {"K_O", SDLK_O},    {"K_P", SDLK_P},
    {"K_F", SDLK_F},    {"K_G", SDLK_G},
    {"K_H", SDLK_H},    {"K_J", SDLK_J},
    {"K_K", SDLK_K},    {"K_L", SDLK_L},
    {"K_M", SDLK_M},    {"K_N", SDLK_N},
    {"K_B", SDLK_B},    {"K_V", SDLK_V},
    {"K_C", SDLK_C},    {"K_X", SDLK_X},
    {"K_Z", SDLK_Z},
};

static unordered_map<char*, int> gl_objects{
    {"DRAW_QUADS", GL_QUADS},
    {"DRAW_TRIANGLES", GL_TRIANGLES},
};