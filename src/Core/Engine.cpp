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
    glEnable(GL_TEXTURE_2D);
    assets = Assets();

    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float)8 / (float)6, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0f, 0.0f, -4.0f);
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
    source.registerFunc("rotate", rotate);
    source.registerFunc("loadTexture", loadTexture);
    source.registerFunc("useTexture", useTexture);
    source.registerFunc("texCoord", texCoord);
    source.registerFunc("mousePos", mousePos);
    source.registerFunc("mouseBtn", mouseBtn);

    source.init(path);

    source.callFunc("g_init");
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
            if(e.type == SDL_WINDOWEVENT){
                int w,h;
                SDL_GetWindowSize(window, &w, &h);
                glViewport(0,0,w,h);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(30, (float)w / (float)h, 0.1, 100.0);
                glMatrixMode(GL_MODELVIEW);
            }
        }
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

int Engine::btn(lua_State* L) {
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "Error: Expected 1 argument for btn (button_id)");
    }

    try {
        int key = luaL_checkinteger(L, 1);
        
        const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
        
        SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
        
        bool isPressed = currentKeyState[scancode];
        lua_pushboolean(L, isPressed);
        
        return 1;
    } 
    catch (const std::exception& error) {
        return luaL_error(L, "btn error: %s", error.what());
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

    return 0;
}

int Engine::rotate(lua_State* L){
    if (lua_gettop(L) != 4) {
        lua_pushstring(L, "Error: Expected 4 arguments (x, y, z)");
        lua_error(L); 
        return 0;
    }

    try{
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);
        float angle = luaL_checknumber(L, 4);

        glRotatef(angle, x, y, z);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
    return 0;
}

int Engine::loadTexture(lua_State* L){
    if (lua_gettop(L) != 1) {
        lua_pushstring(L, "Error: Expected 1 argument (path)");
        lua_error(L); 
        return 0;
    }
    
    try {
        const char* path = luaL_checkstring(L, 1);
        int id = assets.load(path);
        
        lua_pushinteger(L, id);
        
        return 1;
    } 
    catch(exception& error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        lua_pushnil(L); 
        return 1;
    }

    return 0;
}

int Engine::useTexture(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }

    try{
        int id = luaL_checkinteger(L, 1);
        GLuint texture = assets.get(id);

        if (texture == 0) {
            lua_pushstring(L, "Error: Invalid texture ID");
            lua_error(L); 
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, texture);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::texCoord(lua_State* L){
    if (lua_gettop(L) != 2){
        lua_pushstring(L, "Error: Expected 1 arguments (x, y)");
        lua_error(L); 
        return 0;
    }

    try{
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        glTexCoord2f(x,y);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::mouseBtn(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }
    try{
        int button = luaL_checkinteger(L, 1);
        
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        
        bool isPressed = false;
        switch (button) {
            case 1: isPressed = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)); break;
            case 2: isPressed = (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)); break;
            case 3: isPressed = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)); break;
            default:
                lua_pushstring(L, "Error: Invalid button (1=left, 2=middle, 3=right)");
                lua_error(L);
                return 0;
        }

        lua_pushboolean(L, isPressed);
        return 1; 
    } catch (exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }


    return 1;
}

int Engine::mousePos(lua_State* L) {
    try {
        int x, y;
        SDL_GetMouseState(&x, &y);

        lua_pushinteger(L, x);
        lua_pushinteger(L, y);

        return 2; 
    } 
    catch (const exception& error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        lua_pushnil(L);
        return 1;
    }
}