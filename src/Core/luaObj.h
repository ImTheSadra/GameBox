#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

using namespace std;

static unordered_map<char*, int> lua_keys{
    {"K_SPACE", SDLK_SPACE},
    {"K_TAB", SDLK_TAB},
    {"K_UP", SDLK_UP},
    {"K_DOWN", SDLK_DOWN},
    {"K_RIGHT", SDLK_RIGHT},
    {"K_LEFT", SDLK_LEFT},

    {"K_W", SDLK_w},    {"K_D", SDLK_d},
    {"K_A", SDLK_a},    {"K_S", SDLK_s},
    {"K_Q", SDLK_q},    {"K_E", SDLK_e},
    {"K_R", SDLK_r},    {"K_T", SDLK_t},
    {"K_U", SDLK_u},    {"K_I", SDLK_i},
    {"K_O", SDLK_o},    {"K_P", SDLK_p},
    {"K_F", SDLK_f},    {"K_G", SDLK_g},
    {"K_H", SDLK_h},    {"K_J", SDLK_j},
    {"K_K", SDLK_k},    {"K_L", SDLK_l},
    {"K_M", SDLK_m},    {"K_N", SDLK_n},
    {"K_B", SDLK_b},    {"K_V", SDLK_v},
    {"K_C", SDLK_c},    {"K_X", SDLK_x},
    {"K_Z", SDLK_z},
};