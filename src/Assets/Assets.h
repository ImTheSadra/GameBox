#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include "../MException.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include "Font.h"

using namespace std;

class Assets {
public:
    Assets();
    int load(const char* path);
    int loadFromSurface(SDL_Surface* surface);
    GLuint get(int id);

    // void setFont(const char* path);
    // int renderText(const char* text, SDL_Color color, const int size);
private:
    unordered_map<int, GLuint> textures;
    int currentId;
    // GFont* font;
};

