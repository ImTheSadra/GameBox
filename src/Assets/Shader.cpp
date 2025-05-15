#include "Shader.h"

void Shader::init() {
    compile(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
}

Shader::~Shader() {
    if (program_) {
        glDeleteProgram(program_);
    }
}

void Shader::use() const {
    glUseProgram(program_);
}

void Shader::compile(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_);
    checkLinkErrors(program_);

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    try {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);
        compile(vertexCode.c_str(), fragmentCode.c_str());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Shader loading error: " << e.what() << std::endl;
        return false;
    }
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
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        throw MException(__LINE__, __FILE__, 
            "Shader compilation error (" + type + "):\n" + infoLog);
    }
}

void Shader::checkLinkErrors(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        auto e = MException(__LINE__, __FILE__, 
            "Shader program linking error:\n" + (string)infoLog);
        e.ShowMessageBox();
        throw e;
    }
}

std::string Shader::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw MException(__LINE__, __FILE__, 
            "Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const GLfloat* value) const {
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string &name, const GLfloat* value) const {
    glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, value);
}