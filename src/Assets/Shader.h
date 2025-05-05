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
    void use();
    void set(char* vx, char* fg);
private:
    const char* defaultVX = R"glsl(
        #version 120
        attribute vec3 position;
        attribute vec3 color;
        varying vec3 vColor;
        uniform mat4 MVP;
        
        void main() {
            vColor = color;
            gl_Position = MVP * vec4(position, 1.0);
        }
    )glsl";

    const char* defaultFG = R"glsl(
        #version 120
        varying vec3 vColor;
        
        void main() {
            gl_FragColor = vec4(vColor, 1.0);
        }
    )glsl";

    char* sourceFG, sourceVX;

    void checkCompileErrors(GLuint shader, const string& type) {
        GLint success;
        GLchar infoLog[1024];
        
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                ostringstream oss;
                oss << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                << infoLog << "\n -- --------------------------------------------------- -- " << endl;
                auto e = MException(__LINE__, __FILE__, oss.str());
                e.ShowMessageBox();
                throw e;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                ostringstream oss;
                oss << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                << infoLog << "\n -- --------------------------------------------------- -- " << endl;
                auto e = MException(__LINE__, __FILE__, oss.str());
                e.ShowMessageBox();
                throw e;
            }
        }
    }

    GLuint program;
};