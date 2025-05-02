#include "Engine.h"

Engine::Engine(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
        throw e;
    }

    window = SDL_CreateWindow(
        "GameBox - 2D :)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if(!window){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
        throw e;
    }

    renderer = SDL_CreateRenderer(window, 0, 0);
    if(!renderer){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
        throw e;
    }
}

void Engine::init(){
    for(auto &obj:lua_keys){
        source.setInt(obj.first, obj.second);
    }

    source.registerFunc("fill", fill);
    source.registerFunc("color", color);
    source.registerFunc("line", line);
    source.registerFunc("btn", btn);
    source.registerFunc("rect", rect);

    source.init("main.lua");
}

Engine::~Engine(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Engine::run(){
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){running = false;}
            if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
                keyState = (Uint8*)SDL_GetKeyboardState(NULL);
            }
        }
        // SDL_SetRenderDrawColor(renderer, 200, 100, 200, 255);
        // SDL_RenderClear(renderer);

        source.callFunc("g_loop");
        
        SDL_RenderPresent(renderer);
    }
}


//custom functions 


int Engine::color(lua_State* L){
    if (lua_gettop(L) != 3) {
        lua_pushstring(L, "Error: Expected 3 arguments for fill (r, g, b).");
        lua_error(L); 
        return 0;
    }

    try{
        int r = luaL_checkinteger(L, 1);
        int g = luaL_checkinteger(L, 2); 
        int b = luaL_checkinteger(L, 3); 
        if (r > 255){r = 255;}
        if (g > 255){g = 255;}
        if (b > 255){b = 255;}
        if (r < 0){r = 0;}
        if (g < 0){g = 0;}
        if (b < 0){b = 0;}
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    
    return 0;
}

int Engine::fill(lua_State* L){
    SDL_RenderClear(renderer);

    return 0;
}

int Engine::line(lua_State* L){
    if (lua_gettop(L) != 4) {
        lua_pushstring(L, "Error: Expected 4 arguments for line (x1, y1, x2, y2).");
        lua_error(L); 
        return 0;
    }
    try{
        int x1 = (int)luaL_checknumber(L, 1);
        int y1 = (int)luaL_checknumber(L, 2);
        int x2 = (int)luaL_checknumber(L, 3);
        int y2 = (int)luaL_checknumber(L, 4);

        SDL_RenderDrawLine(renderer, x1,y1, x2,y2);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::btn(lua_State* L){
    if (lua_gettop(L) != 1) {
        lua_pushstring(L, "Error: Expected 1 argument for btn (button_id)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!keyState){lua_pushboolean(L, false);return 0;}
        int key = luaL_checkinteger(L, 1);
        bool state = keyState[SDLK_SPACE];
        cout << state;
        lua_pushboolean(L, state);
    } catch (exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::rect(lua_State* L) {
    if (lua_gettop(L) != 4) {
        lua_pushstring(L, "Error: Expected 4 arguments (x1, y1, x2, y2)");
        lua_error(L); 
        return 0;
    }

    try {
        if (!renderer) {
            lua_pushstring(L, "Error: Renderer not initialized");
            lua_error(L);
            return 0;
        }

        int x1 = luaL_checknumber(L, 1);
        int y1 = luaL_checknumber(L, 2);
        int x2 = luaL_checknumber(L, 3);
        int y2 = luaL_checknumber(L, 4);

        SDL_Rect r{
            std::min(x1, x2), 
            std::min(y1, y2),
            std::abs(x2 - x1),
            std::abs(y2 - y1)
        };
        SDL_RenderFillRect(renderer, &r);
    }
    catch (exception error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}