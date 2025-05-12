#include <SDL3/SDL.h>
#include <iostream>
#include "Source.h"
#include "../MException.hpp"
#include "luaObj.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
;
static Uint8* keyState;

using namespace std;


class Engine{
public:
    Engine();
    ~Engine();
    void init(const char* path);
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
    static int translate(lua_State* L);
    static int setCam(lua_State* L);
private:
    static void handleError(int line, string file);
    bool running = true;
    Source source;
    static float CamX, CamY, CamZ, TarX, TarY, TarZ;
    void updateView();
};