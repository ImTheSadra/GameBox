#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <sstream>
#include <iostream>
#include "../MException.hpp"

using namespace std;

class Shader{
public:
    Shader();
    ~Shader();
    void use() const;
    void set(const char* vx, const char* fg);
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint getProgramID() const;
    GLuint compileShader(GLenum type, const char* source);

private:
    const char* defaultVX = 
        "#version 120\n"
        "attribute vec3 position;\n"
        "attribute vec3 color;\n"
        "varying vec3 vColor;\n"
        "uniform mat4 MVP;\n"
        
        "void main() {\n"
            "vColor = color;\n"
            "gl_Position = MVP * vec4(position, 1.0);\n"
        "}\n";

    const char* defaultFG = 
        "#version 120"
        "varying vec3 vColor;\n"
        
        "void main() {\n"
            "gl_FragColor = vec4(vColor, 1.0);\n"
        "}";

    char* sourceFG, sourceVX;

    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);
    string readFile(const std::string& filepath);

    GLuint program;
};