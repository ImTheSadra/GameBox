#include "Assets.h"

Assets::Assets() : currentId(0) {}

int Assets::load(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        MException(__LINE__, __FILE__, "Failed to load image: " + string(path) + " Error: " + IMG_GetError()).ShowMessageBox();
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

    SDL_FreeSurface(surface);

    int id = currentId;
    currentId++;

    return id;
}

GLuint Assets::get(int id) {
    // Find and return the texture ID based on the provided key (id)
    if (textures.find(id) != textures.end()) {
        return textures[id];
    } else {
        // If texture is not found, throw an exception or return an invalid texture ID
        MException(__LINE__, __FILE__, "Texture ID not found: " + to_string(id)).ShowMessageBox();
        return 0;  // Return an invalid texture ID
    }
}
