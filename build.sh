#!/bin/bash

OUTPUT="./bin/GBox"
FLAGS="-Wno-all -Wno-write-strings $(pkg-config --cflags --libs lua glew glu opengl sdl3) -lSDL3_image -lSDL3_ttf"

mkdir -p bin

SOURCE_FILES=$(find src -name "*.cpp" -type f -print0 | xargs -0)

echo "Compiling $SOURCE_FILES..."
g++ $SOURCE_FILES $FLAGS -o "$OUTPUT"

if [ $? -ne 0 ]; then
    echo "Compilation failed with error $?"
    read -p "Press any key to continue..."
    exit $?
fi

echo "Running $OUTPUT..."
"$OUTPUT" test.lua

#read -p "Press any key to continue..."
