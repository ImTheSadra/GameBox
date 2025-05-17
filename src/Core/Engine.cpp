#include "Engine.h"

float Engine::CamX = 0.0f;
float Engine::CamY = 0.0f;
float Engine::CamZ = 3.0f;
float Engine::TarX = 0.0f;
float Engine::TarY = 0.0f;
float Engine::TarZ = 0.0f;

// unordered_map<SDL_JoystickID, SDL_Joystick*> Joysticks = {};

Engine::Engine(){
    int x = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    
    if (x < 0){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
    }

    if (!TTF_Init()){
        auto e = MException(__LINE__, __FILE__, SDL_GetError());
        e.ShowMessageBox();
    }

    window = SDL_CreateWindow(
        "GameBox - 3D :)",
        800, 600, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if(!window){handleError(__LINE__, __FILE__);}

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    ctx = SDL_GL_CreateContext(window);
    if (!ctx){handleError(__LINE__, __FILE__);}

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
    glClearColor(0.2f, 0.2f, 0.2f, 0.7f);

    handleError(__LINE__, __FILE__);
}

void Engine::handleError(int line, string file) {
    const char* sdlError = SDL_GetError();
    if (sdlError && sdlError[0] != '\0') {
        auto e = MException(line, file.c_str(), ("SDL Error\t" + (string)sdlError).c_str());
        e.ShowMessageBox();
        SDL_ClearError();
    }

    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR) {
        string glErrorStr;
        switch (glErr) {
            case GL_INVALID_ENUM:      glErrorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     glErrorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: glErrorStr = "GL_INVALID_OPERATION - check that you used functions currectly"; break;
            case GL_STACK_OVERFLOW:   glErrorStr = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:   glErrorStr = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:     glErrorStr = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: glErrorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                   glErrorStr = "Unknown OpenGL error";
        }
        auto e = MException(line, file.c_str(), ("OpenGL Error\t"+glErrorStr).c_str());
        e.ShowMessageBox();
    }
}

void Engine::handleLuaError(lua_State* L){
    lua_Debug debug;
    lua_getstack(L, 1, &debug);
    lua_getinfo(L, "nSl", &debug);

    handleError(debug.currentline, debug.source);
}

void Engine::init(const char* path, bool debug){
    for(auto &obj:lua_keys){
        source.setInt(obj.first, obj.second);
    }

    for(auto &obj:gl_objects){
        source.setInt(obj.first, obj.second);
    }

    source.registerFunc("fill", fill);
    source.registerFunc("color", color);
    source.registerFunc("vertex", vertex3);
    source.registerFunc("btn", btn);
    source.registerFunc("beginDraw", beginDraw);
    source.registerFunc("endDraw", endDraw);
    source.registerFunc("rotate", rotate);
    source.registerFunc("loadTexture", loadTexture);
    source.registerFunc("useTexture", useTexture);
    source.registerFunc("texCoord", texCoord);
    source.registerFunc("mousePos", mousePos);
    source.registerFunc("mouseSetPos", mouseSetPos);
    source.registerFunc("mouseBtn", mouseBtn);
    source.registerFunc("title", title);
    source.registerFunc("camera", setCam);
    source.registerFunc("listJoysticks", listJoysticks);
    source.registerFunc("joyName", getJoystickName);
    source.registerFunc("joyOpen", joystickOpen);
    source.registerFunc("joyRumble", joystickRumble);

    source.init(path);

    if(debug){
        cout << "\n=== Device Information ===" << endl;

        cout << "SDL Video Driver: " << SDL_GetCurrentVideoDriver() << endl;
        cout << "SDL Render Driver: " << SDL_GetRenderDriver(0) << endl;

        cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
        cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        cout << "Vendor: " << glGetString(GL_VENDOR) << endl;;
        cout << "Renderer: " << glGetString(GL_RENDERER) << endl;

        cout << "\nSupported Extensions:" << endl;
        if (GLEW_VERSION_4_6) cout << "OpenGL 4.6 supported" << endl;
        if (GLEW_ARB_shader_storage_buffer_object) cout << "SSBO supported" << endl;
        if (GLEW_ARB_compute_shader) cout << "Compute Shaders supported" << endl;

        GLint maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        cout << "\nMax Texture Size: " << maxTextureSize << "x" << maxTextureSize << endl;

        cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
        
        cout << "==========================" << endl << endl;
    }

    shader = new Shader();
    shader->init();

    source.callFunc("g_init", {});
}

Engine::~Engine(){
    for(auto j:Joysticks){
        SDL_CloseJoystick(j.second);
    }

    SDL_GL_DestroyContext(ctx);
    SDL_DestroyWindow(window);
}

void Engine::updateView() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        CamX, CamY, CamZ,
        TarX, TarY, TarZ,
        0, 1, 0
    );
}

void Engine::run(){
    SDL_Event event;
    int e,s,time=0;
    while(running){
        s = SDL_GetTicks();
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){running = false;}
            if(event.type == SDL_EVENT_KEY_DOWN){
                if(source.functionExists("g_keydown")){
                    vector<LuaValue> args = {LuaValue((int)event.key.key)};
                    source.callFunc("g_keydown", args);
                }
            } else if(event.type == SDL_EVENT_KEY_UP){
                if(source.functionExists("g_keyup")){
                    vector<LuaValue> args = {LuaValue((int)event.key.key)};
                    source.callFunc("g_keyup", args);
                }
            } else if(event.type == SDL_EVENT_WINDOW_RESIZED){
                int w,h;
                SDL_GetWindowSize(window, &w, &h);
                glViewport(0,0,w,h);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(30, (float)w / (float)h, 0.1, 100.0);
                glMatrixMode(GL_MODELVIEW);
                if(source.functionExists("g_wResize")){
                    vector<LuaValue> arguments = {LuaValue((double)w), LuaValue((double)h)};
                    source.callFunc("g_wResize", arguments);
                }
            } else if (event.type == SDL_EVENT_JOYSTICK_ADDED){
                if(source.functionExists("g_joystickAdded")){
                    source.callFunc("g_joystickAdded", {LuaValue((int)event.jdevice.which)});
                }
            } else if (event.type == SDL_EVENT_JOYSTICK_REMOVED){
                if(source.functionExists("g_joystickRemoved")){
                    source.callFunc("g_joystickRemoved", {LuaValue((int)event.jdevice.which)});
                }
            } else if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN){
                if(source.functionExists("g_joykeydown")){
                    source.callFunc("g_joykeydown", 
                        {LuaValue((int)event.jbutton.which), LuaValue((int)event.jbutton.button)});
                }
            } else if (event.type == SDL_EVENT_JOYSTICK_BUTTON_UP){
                if(source.functionExists("g_joykeyup")){
                    source.callFunc("g_joykeyup", 
                        {LuaValue((int)event.jbutton.which), LuaValue((int)event.jbutton.button)});
                }
            } else if (event.type == SDL_EVENT_JOYSTICK_AXIS_MOTION){
                if(source.functionExists("g_joyAxisMove")){
                    float normalized = event.jaxis.value / 32768.0f;
                    int axis = 0;
                    if (!event.jaxis.axis){axis = event.jaxis.axis;}
                    if (abs(normalized) < 0.2f) {
                        normalized = 0.0f;
                    }
                    source.callFunc("g_joyAxisMove", {LuaValue((int)event.jaxis.which), LuaValue((float)normalized), LuaValue(0)});
                }
            }
        }
        updateView();
        source.callFunc("g_loop", {LuaValue(time)});
        // handleError(__LINE__, __FILE__);

        SDL_GL_SwapWindow(window);
        e = SDL_GetTicks();
        time = (e-s)/2;
        SDL_Delay(time);
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
        glColor3f(r,g,b);
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
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
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
        if (!window){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        int key = luaL_checkinteger(L, 1);
        SDL_Keycode keycode = static_cast<SDL_Keycode>(key);
        SDL_Keymod keymod = SDL_GetModState();

        SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, &keymod);
        if (scancode == SDL_SCANCODE_UNKNOWN) {
            lua_pushboolean(L, false);
            return 1;
        }

        auto state = SDL_GetKeyboardState(NULL);
        bool isPressed = (state[scancode] != 0);
        lua_pushboolean(L, isPressed);
        return 1;
    } 
    catch (const exception& error) {
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
    }
    catch (exception error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::beginDraw(lua_State* L){
    if(lua_gettop(L) != 1){
        lua_pushstring(L, "Error: 'beginDraw' Expected 1 argument (DRAW_MODE)");
        lua_error(L);
        return 0;
    }

    try {
        if (!ctx) {
            lua_pushstring(L, "Error: opengl is not initialized");
            lua_error(L);
            return 0;
        }
        
        double MODE = luaL_checknumber(L, 1);

        glBegin(MODE);
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
            lua_pushstring(L, "Error: 'endDraw' opengl is not initialized");
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
        lua_pushstring(L, "Error: 'rotate' Expected 4 arguments (x, y, z)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);
        float angle = luaL_checknumber(L, 4);

        glRotatef(angle, x, y, z);
        handleLuaError(L);
    } catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
    return 0;
}

int Engine::loadTexture(lua_State* L){
    if (lua_gettop(L) != 1) {
        lua_pushstring(L, "Error: 'loadTexture' Expected 1 argument (path)");
        lua_error(L); 
        return 0;
    }
    
    try {
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
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
        lua_pushstring(L, "Error: 'useTexture' Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
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
        lua_pushstring(L, "Error: 'texCoord' Expected 1 arguments (x, y)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
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
        lua_pushstring(L, "Error: 'mouseBtn' Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }
    try{
        if (!window){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        int button = luaL_checkinteger(L, 1);
        
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        
        bool isPressed = false;
        switch (button) {
            case 1: isPressed = (mouseState & SDL_BUTTON_LMASK); break;
            case 2: isPressed = (mouseState & SDL_BUTTON_MMASK); break;
            case 3: isPressed = (mouseState & SDL_BUTTON_RMASK); break;
            case 4: isPressed = (mouseState & SDL_BUTTON_X1MASK); break;
            case 5: isPressed = (mouseState & SDL_BUTTON_X2MASK); break;
            default:
                lua_pushstring(L, "Error: Invalid button (1=left, 2=middle, 3=right, 4=xbtn, 5=xbtn)");
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
        if (!window){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        float x, y;
        SDL_GetMouseState(&x,&y);

        lua_pushnumber(L, x);
        lua_pushnumber(L, y);

        return 2; 
    } 
    catch (const exception& error) {
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        lua_pushnil(L);
        return 1;
    }
}

int Engine::title(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: 'title' Expected 1 arguments (title)");
        lua_error(L); 
        return 0;
    }
    try{
        if (!ctx){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        string title = luaL_checkstring(L, 1);
        SDL_SetWindowTitle(window, title.c_str());

        return 0; 
    } catch (exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }


    return 0;
}

int Engine::setCam(lua_State* L){
    if (lua_gettop(L) != 6){
        lua_pushstring(L, "Error: 'camera' Expected 6 arguments (x,y,z,tx,ty,tz)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!ctx){
            lua_pushstring(L, "OpenGL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);
        float tx = luaL_checknumber(L, 4);
        float ty = luaL_checknumber(L, 5);
        float tz = luaL_checknumber(L, 6);

        CamX = x; CamY = y; CamZ = z;
        TarX = tx; TarY = ty; TarZ = tz;
    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
    return 0;
}

int Engine::mouseSetPos(lua_State* L){
    if (lua_gettop(L) != 2){
        lua_pushstring(L, "Error: 'mouseSetPos' Expected 2 arguments (x,y)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!window){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        double x = luaL_checknumber(L, 1);
        double y = luaL_checknumber(L, 2);
        SDL_WarpMouseInWindow(window, x, y);

    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
    return 0;
}

int Engine::mouseVisible(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: 'mouseVisible' Expected 1 arguments (isVisible)");
        lua_error(L); 
        return 0;
    }

    try{
        if (!ctx){
            lua_pushstring(L, "SDL is not initilized yet.");
            lua_error(L); 
            return 0;
        }
        bool isv = luaL_checkinteger(L, 1);
        int result = SDL_SetWindowRelativeMouseMode(window, isv);

        if (result != 0) {
            lua_pushstring(L, SDL_GetError());
            lua_error(L);
            return 0;
        }

        return 0;
    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }
    return 0;
}

int Engine::listJoysticks(lua_State* L){
    try{
        vector<SDL_JoystickID> ids{};
    
        SDL_JoystickID* joys = SDL_GetJoysticks(NULL);
        if (joys) {
            for (int i = 0; joys[i]; i++) {
                ids.push_back(joys[i]);
            }
            SDL_free(joys);
        }

        lua_createtable(L, ids.size(), 0);
        for (size_t i = 0; i < ids.size(); i++) {
            lua_pushinteger(L, ids[i]);
            lua_rawseti(L, -2, i + 1);
        }

        return 1;
    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::getJoystickName(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: 'joyName' Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }

    try{
        SDL_JoystickID instanceId = static_cast<SDL_JoystickID>(luaL_checkinteger(L, 1));
    
        auto it = Joysticks.find(instanceId);
        if (it == Joysticks.end()) {
            return luaL_error(L, "Error: Joystick not opened");
        }
        
        const char* name = SDL_GetJoystickName(it->second);
        if (!name) {
            return luaL_error(L, "Error: Could not get joystick name");
        }
        
        lua_pushstring(L, name);

        return 1;
    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::joystickOpen(lua_State* L){
    if (lua_gettop(L) != 1){
        lua_pushstring(L, "Error: 'joyOpen' Expected 1 arguments (id)");
        lua_error(L); 
        return 0;
    }

    try{
        SDL_JoystickID instanceId = static_cast<SDL_JoystickID>(luaL_checkinteger(L, 1));

        SDL_Joystick* joystick = SDL_OpenJoystick(instanceId);
        if (!joystick) {
            lua_pushstring(L, ("SDL JoyStick Error : " + (string)SDL_GetError()).c_str());
            lua_error(L);
            return 0;
        }
        Joysticks.insert(pair<SDL_JoystickID, SDL_Joystick*>(instanceId, joystick));
        lua_pushinteger(L, (int)instanceId);
        return 1;
    }catch(exception error){
        auto e = MException(__LINE__, __FILE__, error.what());
        e.ShowMessageBox();
        throw e;
    }

    return 0;
}

int Engine::joystickRumble(lua_State* L) {
    if (lua_gettop(L) != 4) {
        return luaL_error(L, "Error: 'joyRumble' expected 4 arguments (id, L_strength, R_strength, duration_ms)");
    }

    try {
        SDL_JoystickID instanceId = static_cast<SDL_JoystickID>(luaL_checkinteger(L, 1));
        float ls = static_cast<float>(luaL_checknumber(L, 2));
        float rs = static_cast<float>(luaL_checknumber(L, 3));
        Uint32 duration_ms = static_cast<Uint32>(luaL_checkinteger(L, 4));

        if (ls < 0.0f || ls > 1.0f || rs < 0.0f || rs > 1.0f) {
            return luaL_error(L, "Error: strength must be between 0.0 and 1.0");
        }
        
        auto it = Joysticks.find(instanceId);
        if (it == Joysticks.end()) {
            return luaL_error(L, "Error: Joystick not opened or invalid ID");
        }

        SDL_Joystick* joystick = it->second;

        if (!SDL_RumbleJoystick(joystick, 
            static_cast<Uint16>(ls * 0xFFFF),
            static_cast<Uint16>(rs * 0xFFFF),
            duration_ms) != 0) 
        {
        return luaL_error(L, ("SDL Error: " + std::string(SDL_GetError())).c_str());
        }

        lua_pushboolean(L, true);
        return 1;

    } catch (const std::exception& error) {
        return luaL_error(L, ("Error in joyRumble: " + std::string(error.what())).c_str());
    } catch (...) {
        return luaL_error(L, "Unknown error in joyRumble");
    }
}

int Engine::joystickGetAxis(lua_State* L){
    if (lua_gettop(L) != 1) {
        return luaL_error(L, "Error: 'joyAxis' expected 4 arguments (id)");
    }

    try{
        SDL_JoystickID instanceId = static_cast<SDL_JoystickID>(luaL_checkinteger(L, 1));

        auto it = Joysticks.find(instanceId);
        if (it == Joysticks.end()) {
            return luaL_error(L, "Error: Joystick not opened or invalid ID");
        }
        float x,y;
        x = SDL_GetJoystickAxis(it->second, 0);
        y = SDL_GetJoystickAxis(it->second, 1);

        lua_pushnumber(L, x/32767);
        lua_pushnumber(L, y/32767);

        return 1;
    } catch (exception error){
        MException(__LINE__, __FILE__, error.what());
    }

    return 0;
}