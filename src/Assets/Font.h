#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include "Assets.h"

class GFont{
public:
    GFont(const char* path, const int size);
    ~GFont();

    SDL_Surface* write(string text, SDL_Color color, const int size);
private:
    TTF_Font* font;
    int size;
};