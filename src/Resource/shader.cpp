
#include <glad/glad.h>
#include "shader.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>

char* Shader::ReadSource(const char *filename) {
    std::ifstream fin(filename);
    std::istreambuf_iterator<char> begin(fin);
    std::istreambuf_iterator<char> end;
    std::string filecontent(begin, end);
    char* buffer = new char[filecontent.size()+5];
    memcpy(buffer, filecontent.data(), filecontent.size());
    buffer[filecontent.size()] = '\0';
    return buffer;
}
void Shader::CheckCompileError(unsigned int shader, std::string type) {
    int success;
    char infolog[1024];
    if(type != "ShaderProgram"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 1024, NULL, infolog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
            std::cout << infolog << std::endl;
            std::cout << "------------------------------------------------- " << std::endl;
        }
    }
    else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader, 1024, NULL, infolog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
            std::cout << infolog << std::endl;
            std::cout << "------------------------------------------------- " << std::endl;
        }
    }
}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char* geometryPath) {
    GLuint vertexShader = 0;
    if(vertexPath != nullptr) {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        char *vertexShaderSource = ReadSource(vertexPath);
//        printf("vertexShader:\n%s\n", vertexShaderSource);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        delete[] vertexShaderSource;
        CheckCompileError(vertexShader, "VertexShader");
    }

    GLuint fragmentShader = 0;
    if(fragmentPath != nullptr) {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        char *fragmentShaderSource = ReadSource(fragmentPath);
//        printf("fragmentShader:\n%s\n", fragmentShaderSource);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        delete[] fragmentShaderSource;
        CheckCompileError(fragmentShader, "FragmentShader");
    }

    GLuint geometryShader = 0;
    if(geometryPath != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        char *geometryShaderSource = ReadSource(geometryPath);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        delete[] geometryShaderSource;
        CheckCompileError(geometryShader, "GeometryShader");
    }

    this->shaderProgram = glCreateProgram();
    if(vertexPath != nullptr) glAttachShader(shaderProgram, vertexShader);
    if(fragmentPath != nullptr) glAttachShader(shaderProgram, fragmentShader);
    if(geometryPath != nullptr) glAttachShader(shaderProgram, geometryShader);
    glLinkProgram(shaderProgram);
    CheckCompileError(shaderProgram, "ShaderProgram");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(shaderProgram);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec2(const std::string &name, glm::vec2 value) const {
    glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y);
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z, value[3]);
}