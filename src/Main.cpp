#include "Core/Engine.h"

#define __G_VERSION__ 1.4
#define __G_NAME__ "GameBox"

void run(char* filepath, bool debug){
    Engine engine = Engine();
    engine.init(filepath, debug);
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

    bool debug = false;
    
    if (argc < 2) {
        MException err = MException(__LINE__, __FILE__, 
            "Usage: gbox <script.lua> [other options]\n"
            "Options:\n"
            "  -d, --debug   Enable debug mode\n"
            "  -h, --help    Show this help message");
        err.ShowMessageBox();
        LocalFree(argvW);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        int size = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, NULL, 0, NULL, NULL);
        char* arg = new char[size];
        WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, arg, size, NULL, NULL);
        
        if (strcmp(arg, "-d") == 0 || strcmp(arg, "--debug") == 0) {
            debug = true;
            delete[] arg;
            cout << "debugging mode is on" << endl;
            continue;
        }
        
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            MException help = MException(__LINE__, __FILE__, 
                "GameBox Engine v1\n"
                "Usage: gbox <script.lua> [options]\n\n"
                "Options:\n"
                "  -d, --debug   Enable debug mode\n"
                "  -h, --help    Show this help message");
            help.ShowMessageBox();
            delete[] arg;
            LocalFree(argvW);
            return 0;
        }
        
        delete[] arg;
    }

    char* filepath = nullptr;
    for (int i = 1; i < argc; i++) {
        int size = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, NULL, 0, NULL, NULL);
        char* arg = new char[size];
        WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, arg, size, NULL, NULL);
        
        if (arg[0] != '-') {
            filepath = new char[size];
            strcpy(filepath, arg);
            delete[] arg;
            break;
        }
        delete[] arg;
    }

    if (!filepath) {
        MException err = MException(__LINE__, __FILE__, "No script file specified");
        err.ShowMessageBox();
        LocalFree(argvW);
        return 1;
    }

    run(filepath, debug);
    
    delete[] filepath;
    LocalFree(argvW);
    return 0;
}

#else

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <script.lua> \n", argv[0]);
        return 1;
    }
    
    run(argv[1], true);
    return 0;
}

#endif