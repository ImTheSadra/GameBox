#include "Assets.h"

Assets::Assets() : currentId(0) {}

int Assets::load(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        MException(__LINE__, __FILE__, "Failed to load image: " + string(path) + " Error: " + SDL_GetError()).ShowMessageBox();
        return -1; 
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D); 

    textures[currentId] = textureID;

    SDL_DestroySurface(surface);

    int id = currentId;
    currentId++;

    return id;
}

GLuint Assets::get(int id) {
    if (textures.find(id) != textures.end()) {
        return textures[id];
    } else {
        MException(__LINE__, __FILE__, "Texture ID not found: " + to_string(id)).ShowMessageBox();
        return 0;
    }
}
