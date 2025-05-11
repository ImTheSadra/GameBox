#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : program(0) {
    set(defaultVX, defaultFG);
}

Shader::~Shader() {
    if (program) {
        glDeleteProgram(program);
    }
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    try {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);
        set(vertexCode.c_str(), fragmentCode.c_str());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return false;
    }
}

void Shader::set(const char* vx, const char* fg) {
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }

    GLuint vs = compileShader(GL_VERTEX_SHADER, vx);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fg);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    checkLinkErrors(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::use() const {
    glUseProgram(program);
}

GLuint Shader::getProgramID() const {
    return program;
}

GLuint Shader::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::checkLinkErrors(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" 
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}