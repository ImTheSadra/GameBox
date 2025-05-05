#include "Core/Engine.h"

#define __G_VERSION__ 1
#define __G_NAME__ "GameBox"

void run(char* filepath){
    Engine engine = Engine();
    engine.init(filepath);
    engine.run();
}

#ifdef _WIN32

#include <windows.h>
#include <shellapi.h>

int WINAPI WinMain(
    HINSTANCE,
    HINSTANCE,
    LPSTR lpCmdLine,
    int
) {
    int argc;
    LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    
    if (argc < 2) {
        MessageBoxA(NULL, "Usage: GameBoxEngine <script.lua>", "Error", MB_ICONERROR);
        return 1;
    }

    int size = WideCharToMultiByte(CP_UTF8, 0, argvW[1], -1, NULL, 0, NULL, NULL);
    char* filepath = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, argvW[1], -1, filepath, size, NULL, NULL);

    run(filepath);
    
    delete[] filepath;
    LocalFree(argvW);
    return 0;
}

#else

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <script.lua>\n", argv[0]);
        return 1;
    }
    
    run(argv[1]);
    return 0;
}

#endif