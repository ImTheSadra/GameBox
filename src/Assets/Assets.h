#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include "../MException.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;

class Assets {
public:
    Assets();
    int load(const char* path);
    GLuint get(int id);
private:
    unordered_map<int, GLuint> textures;
    int currentId;
};

