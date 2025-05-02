#include "Core/Engine.h"

void run(){
    Engine engine = Engine();
    engine.init();
    engine.run();
}

#ifdef _WIN32

#include <windows.h>
int WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR, int
){
    run();
    return 0;
}

#else

int main(){
    run();
    return 0;
}

#endif