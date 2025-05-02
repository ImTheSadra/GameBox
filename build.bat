@echo off
setlocal enabledelayedexpansion

set SDL=D:\code\packages\c++\SDL2\x86_64
set LUA=D:\code\packages\c++\lua
set GLEW=D:\code\packages\c++\glew

set output=bin\GameBox.exe
set FLAGS=-Wno-all -Wno-write-strings -I%SDL%\include -I%LUA%\include -I%GLEW%\include -L%SDL%\lib -L%LUA%\lib -L%GLEW%\lib -lSDL2 -llua -lglew32 -lopengl32 -lglew32mx -lglu32

if not exist "bin" mkdir "bin"

set source=
for /r src %%i in (*.cpp) do set source=!source! "%%i"

echo Compiling !source!...
g++ !source! %FLAGS% -o "%output%"

if errorlevel 1 (
    echo Compilation failed with error %errorlevel%
    pause
    exit /b %errorlevel%
)

echo Running %output%...
"%output%" test.lua

pause