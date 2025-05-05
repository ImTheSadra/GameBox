#include "Shader.h"

Shader::Shader(){
    this->set((char*)this->defaultVX, (char*)this->defaultFG);
}

Shader::~Shader(){
    if (program){
        glDeleteProgram(program);
    }
}

void Shader::set(char* vx, char* fg){
    if(program){glDeleteProgram(program);}
    GLuint vs, fs;

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vx, NULL);
    glCompileShader(vs);
    checkCompileErrors(vs, "VERTEX");

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fg, NULL);
    glCompileShader(fs);
    checkCompileErrors(fs, "FRAGMENT");

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::use(){
    glUseProgram(program);
}