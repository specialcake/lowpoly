//
// Created by dydxh on 10/23/18.
//

#ifndef BREAKOUT_RESOURCE_MANAGER_H
#define BREAKOUT_RESOURCE_MANAGER_H
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

class ResourceManager {
public:
    static GLFWwindow* window;
    static GLfloat lastX, lastY;
    static GLboolean firstMouse;
    static GLboolean Keys[1024];
    // Resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, Model> Models;
    static Camera camera;

//    ResourceManager();

    static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader GetShader(std::string name);

    static Texture2D LoadTexture(const GLchar *file, GLuint format, std::string name);
    static Texture2D MakeTexture(const GLuint width, const GLuint height, GLuint format, GLfloat* data, std::string name);
    static Texture2D GetTexture(std::string name);

    static Model* LoadModel(const GLchar *file, std::string name);
    static Model* GetModel(std::string name);

    static void window_initialize();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void Clear();
private:
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const GLchar *file, GLuint format);
};

#endif //BREAKOUT_RESOURCE_MANAGER_H
