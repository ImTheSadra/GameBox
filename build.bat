@echo off
setlocal enabledelayedexpansion

:: ===== Configuration =====
set SDL=D:\code\packages\c++\SDL3
set LUA=D:\code\packages\c++\lua
set GLEW=D:\code\packages\c++\glew

set output=bin\GBox.exe
set assets_dir=assets
set build_dir=build
set FLAGS=-Wno-all -Wno-write-strings -lgdi32 -I%SDL%\include -I%LUA%\include -I%GLEW%\include -L%SDL%\lib -L%LUA%\lib -L%GLEW%\lib -lSDL3 -llua -lglew32 -lopengl32 -lglew32mx -lglu32 -lSDL3_image

:: ===== Prepare Directories =====
if not exist "bin" mkdir "bin"
if not exist "%build_dir%" mkdir "%build_dir%"

:: ===== Resource Compilation (windres) =====
@REM if exist "%assets%/resources.rc" (
@REM     echo Compiling resources...
@REM     windres "%assets%/resources.rc" -o %build_dir%\resources.o
@REM     if errorlevel 1 (
@REM         echo Resource compilation failed
@REM         pause
@REM         exit /b 1
@REM     )
@REM     set RESOURCE_OBJ=%build_dir%\resources.o
@REM )

:: ===== Compilation =====
set source=
for /r src %%i in (*.cpp) do (
    set obj=%build_dir%\%%~ni.o
    echo Compiling %%~nxi...
    g++ -c "%%i" %FLAGS% -o "!obj!"
    if errorlevel 1 (
        echo Compilation failed on %%~nxi
        pause
        exit /b 1
    )
    set source=!source! "!obj!"
)

:: ===== Linking =====
echo Linking...
g++ !source! %FLAGS% -o "%output%"
xcopy "%GLEW%\lib\*.dll" bin\ /Y
xcopy "%SDL%\bin\*.dll" bin\  /Y
xcopy "%LUA%\lib\*.dll" bin\  /Y

if errorlevel 1 (
    echo Linking failed with error %errorlevel%
    pause
    exit /b %errorlevel%
)

:: ===== Clean =====
del /q %build_dir%

:: ===== Run =====
echo Running %output%...
"%output%" test.lua

pause
