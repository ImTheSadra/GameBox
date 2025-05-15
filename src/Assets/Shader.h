#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include "../MException.hpp"

class Shader {
public:
    void init();
    Shader(){}
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    void compile(const char* vertexSource, const char* fragmentSource);
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint getProgramID() const { return program_; }

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const GLfloat* value) const;
    void setVec3(const std::string &name, const GLfloat* value) const;

private:
    static constexpr const char* DEFAULT_VERTEX_SHADER =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 vColor;\n"
        "uniform mat4 MVP;\n"
        "void main() {\n"
        "    vColor = color;\n"
        "    gl_Position = MVP * vec4(position, 1.0);\n"
        "}\n";

    static constexpr const char* DEFAULT_FRAGMENT_SHADER =
        "#version 330 core\n"
        "in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(vColor, 1.0);\n"
        "}\n";

    GLuint program_ = 0;

    GLuint compileShader(GLenum type, const char* source);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void checkLinkErrors(GLuint program);
    std::string readFile(const std::string& filepath);
};