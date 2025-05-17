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

#include <cstring>
#include <cstdio>

int main(int argc, char* argv[]) {
    bool debug = false;
    
    if (argc < 2) {
        printf("Usage: %s <script.lua> [options]\n"
               "Options:\n"
               "  -d, --debug   Enable debug mode\n"
               "  -h, --help    Show this help message\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
            debug = true;
            continue;
        }
        
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            cout << __G_NAME__ << " Engine v" << __G_VERSION__ << endl;
            printf("Usage: %s <script.lua> [options]\n\n"
                   "Options:\n"
                   "  -d, --debug   Enable debug mode\n"
                   "  -h, --help    Show this help message\n", argv[0]);
            return 0;
        }
    }

    char* filepath = nullptr;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            filepath = argv[i];
            break;
        }
    }

    if (!filepath) {
        printf("Error: No script file specified\n");
        return 1;
    }

    run(filepath, debug);
    return 0;
}

#endif