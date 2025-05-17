#include "Font.h"

GFont::GFont(const char* path, const int size) : size(size){
    font = TTF_OpenFont(path, size);
    if(!font){
        auto e = MException(__LINE__, __FILE__, ("cannot load ttf font : " + (string)SDL_GetError()).c_str());
        e.ShowMessageBox();
    }
}

GFont::~GFont(){
    TTF_CloseFont(font);
}

SDL_Surface* GFont::write(string text, SDL_Color color, const int size){
    if (!font){
        auto e = MException(__LINE__, __FILE__, "font is not initilized");
        e.ShowMessageBox();
    }
    TTF_SetFontSize(font, size);
    return TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
}