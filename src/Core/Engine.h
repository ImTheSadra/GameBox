#include <SDL3/SDL.h>
#include <iostream>
#include "Source.h"
#include "../MException.hpp"
#include "luaObj.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3_ttf/SDL_ttf.h>

;
static Uint8* keyState;

using namespace std;


class Engine{
public:
    Engine();
    ~Engine();
    void init(const char* path, bool debug);
    void run();

    static int fill(lua_State* L);
    static int vertex3(lua_State* L);
    static int color(lua_State* L);
    static int beginDraw(lua_State* L);
    static int endDraw(lua_State* L);
    static int btn(lua_State* L);
    static int rotate(lua_State* L);
    static int loadTexture(lua_State* L);
    static int useTexture(lua_State* L);
    static int texCoord(lua_State* L);
    static int mouseBtn(lua_State* L);
    static int mousePos(lua_State* L);
    static int mouseSetPos(lua_State* L);
    static int mouseVisible(lua_State* L);
    static int title(lua_State* L);
    static int setCam(lua_State* L);
    static int listJoysticks(lua_State* L);
    static int getJoystickName(lua_State* L);
    static int joystickOpen(lua_State* L);
    static int joystickRumble(lua_State* L);
    static int joystickGetAxis(lua_State* L);
private:
    static void handleError(int line, string file);
    static void handleLuaError(lua_State* state);
    bool running = true;
    Source source;
    static float CamX, CamY, CamZ, TarX, TarY, TarZ;
    void updateView();
};