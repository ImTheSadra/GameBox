#include "Engine.h"

Engine::Engine(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
        throw e;
    }

    window = SDL_CreateWindow(
        "GameBox - 3D :)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600, 
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if(!window){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
        throw e;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    ctx = SDL_GL_CreateContext(window);
    if (!ctx){
        if(!window){
            auto e = MException(__LINE__, __FILE__, SDL_GetError());
            e.ShowMessageBox();
            throw e;
        }
    }

    if (glewInit() != GLEW_OK) {
        auto e = MException(__LINE__, __FILE__, "GLEW Initialization Failed!");
        e.ShowMessageBox();
        throw e;
    }
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float)8 / (float)6, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.2f, 0.2f, 0.2f, 0.7f);
}

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

void Engine::handleError(int line, string file) {
    const char* sdlError = SDL_GetError();
    if (sdlError && sdlError[0] != '\0') {
        auto e = MException(line, file.c_str(), ("SDL Error\t" + (string)sdlError).c_str());
        e.ShowMessageBox();
        SDL_ClearError();
    }

    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR) {
        std::string glErrorStr;
        switch (glErr) {
            case GL_INVALID_ENUM:      glErrorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     glErrorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: glErrorStr = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:   glErrorStr = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:   glErrorStr = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:     glErrorStr = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: glErrorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                   glErrorStr = "Unknown OpenGL error";
        }
        // std::cerr << "OpenGL Error (" << file << ":" << line << "): " << glErrorStr << std::endl;
        auto e = MException(__LINE__, __FILE__, ("OpenGL Error\t"+glErrorStr).c_str());
        // glErr = glGetError();
    }

    // GLenum glewErr = glewgeterror();
    // if (glewErr != GLEW_OK) {
    //     const char* glewErrorStr = reinterpret_cast<const char*>(glewGetErrorString(glewErr));
    //     std::cerr << "GLEW Error (" << file << ":" << line << "): " << glewErrorStr << std::endl;
    // }
}

void Engine::init(const char* path){
    for(auto &obj:lua_keys){
        source.setInt(obj.first, obj.second);
    }

    source.registerFunc("fill", fill);
    source.registerFunc("color", color);
    source.registerFunc("vertex", vertex3);
    source.registerFunc("btn", btn);
    source.registerFunc("beginQuad", beginQuad);
    source.registerFunc("endDraw", endDraw);

    source.init(path);
}

Engine::~Engine(){
    SDL_GL_DeleteContext(ctx);
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

        // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // glBegin(GL_QUADS);

        // glVertex3f(-0.5, -0.5, -2.0);
        // glVertex3f(-0.5,  0.5, -2.0);
        // glVertex3f( 0.5, -0.5, -2.0);
        // glVertex3f( 0.5,  0.5, -2.0);

        // glEnd();

        source.callFunc("g_loop");
        
        SDL_GL_SwapWindow(window);
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
        float r = luaL_checknumber(L, 1);
        float g = luaL_checknumber(L, 2); 
        float b = luaL_checknumber(L, 3); 
        if (r > 1){r = 1;}
        if (g > 1){g = 1;}
        if (b > 1){b = 1;}
        if (r < 0){r = 0;}
        if (g < 0){g = 0;}
        if (b < 0){b = 0;}
        // SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        glColor3f(r,g,b);

        handleError(__LINE__, __FILE__);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    
    return 0;
}

int Engine::fill(lua_State* L){
    if (lua_gettop(L) != 3) {
        lua_pushstring(L, "Error: Expected 3 arguments for fill (r, g, b).");
        lua_error(L); 
        return 0;
    }

    try{
        float r = luaL_checknumber(L, 1);
        float g = luaL_checknumber(L, 2); 
        float b = luaL_checknumber(L, 3); 
        if (r > 1){r = 1;}
        if (g > 1){g = 1;}
        if (b > 1){b = 1;}
        if (r < 0){r = 0;}
        if (g < 0){g = 0;}
        if (b < 0){b = 0;}

        glClearColor(r,g,b,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handleError(__LINE__, __FILE__);
    } catch (exception error){
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

int Engine::vertex3(lua_State* L) {
    if (lua_gettop(L) != 3) {
        lua_pushstring(L, "Error: Expected 4 arguments (x, y, z)");
        lua_error(L); 
        return 0;
    }

    try {
        if (!ctx) {
            lua_pushstring(L, "Error: opengl is not initialized");
            lua_error(L);
            return 0;
        }

        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);

        glVertex3f(x,y,z);

        handleError(__LINE__, __FILE__);
    }
    catch (exception error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::beginQuad(lua_State* L){
    try {
        if (!ctx) {
            lua_pushstring(L, "Error: opengl is not initialized");
            lua_error(L);
            return 0;
        }

        glBegin(GL_QUADS);
    }
    catch (exception error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::endDraw(lua_State* L){
    try {
        if (!ctx) {
            lua_pushstring(L, "Error: opengl is not initialized");
            lua_error(L);
            return 0;
        }

        glEnd();
    }
    catch (exception error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
}