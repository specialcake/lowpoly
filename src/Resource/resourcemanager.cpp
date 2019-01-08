//
// Created by dydxh on 10/23/18.
//

#include "resourcemanager.h"
#include "../config.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Direction ResourceManager::dir;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
Skybox* ResourceManager::skybox;
Camera ResourceManager::camera;
Fontdisplay ResourceManager::fontdisplay;
GLfloat ResourceManager::lastX, ResourceManager::lastY;
GLboolean ResourceManager::firstMouse = GL_TRUE;
GLboolean ResourceManager::Keys[1024];
GLFWwindow* ResourceManager::window;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}
Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLuint format, std::string name) {
    Textures[name] = loadTextureFromFile(file, format);
    return Textures[name];
}
Texture2D ResourceManager::MakeTexture(const GLuint width, const GLuint height, GLuint format, GLfloat* data, std::string name, GLuint FilterMin, GLuint FilterMax) {
    if(Textures.count(name)){
        glDeleteTextures(1, &Textures[name].ID);
    }
    Texture2D texture;
    texture.Filter_Min = FilterMin, texture.Filter_Max = FilterMax;
    texture.Internal_Format = texture.Image_Format = format;
    if(data != nullptr){
        texture.Generatefloat(static_cast<GLuint>(width), static_cast<GLuint>(height), data);
    }
    else{
        std::cout << "Can not make texture: " << std::endl;
    }
    Textures[name] = texture;
    return texture;
}
Texture2D ResourceManager::MakeTexture(const GLuint width, const GLuint height, GLuint format, glm::vec3* data, std::string name) {
    if(Textures.count(name)){
        glDeleteTextures(1, &Textures[name].ID);
    }
    Texture2D texture;
    texture.Internal_Format = texture.Image_Format = format;
    if(data != nullptr){
        texture.Generatevec3(static_cast<GLuint>(width), static_cast<GLuint>(height), data);
    }
    else{
        std::cout << "Can not make texture: " << std::endl;
    }
    Textures[name] = texture;
    return texture;
}
Texture2D ResourceManager::MakeTexturebyID(const GLuint &textureID) {
    return Texture2D(textureID);
}

Texture2D ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}
Model* ResourceManager::LoadModel(const GLchar *file, std::string name) {
    Models[name] = Model(file);
    return &Models[name];
}
Model* ResourceManager::GetModel(std::string name) {
    return &Models[name];
};

void ResourceManager::Displayfont(const std::string text, const glm::vec3 pos, const glm::vec3 scale,
                                  const glm::vec3 color) {
    fontdisplay.RenderText(text, pos, scale, color);
}

void ResourceManager::Clear() {

    for (auto iter : Shaders)
        glDeleteProgram(iter.second.shaderProgram);

    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile) {
    Shader shader(vShaderFile, fShaderFile, gShaderFile);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *filepath, GLuint format) {
    Texture2D texture;
    texture.Internal_Format = texture.Image_Format = format;
    int width, height, nrComponents;
    unsigned char* image = stbi_load(filepath, &width, &height, &nrComponents, 0);
    if(image != nullptr){
        texture.Generate(static_cast<GLuint>(width), static_cast<GLuint>(height), image);
    }
    else{
        std::cout << "Texture failed to load at path: " << filepath << std::endl;
    }
    stbi_image_free(image);
    return texture;
}

void ResourceManager::window_initialize() {
    srand(time(0));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);
    if(window == NULL){
        std::cout << "Fail to create GLFW window!" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Fail to initialize glad." << std::endl;
        glfwTerminate();
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ResourceManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<GLfloat>(xpos);
        lastY = static_cast<GLfloat>(ypos);
        firstMouse = GL_FALSE;
    }
    auto xoffset = static_cast<GLfloat>(xpos - lastX);
    auto yoffset = static_cast<GLfloat>(lastY - ypos);
    lastX = static_cast<GLfloat>(xpos);
    lastY = static_cast<GLfloat>(ypos);
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void ResourceManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Keys[key] = GL_FALSE;
    }
}
void ResourceManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
void ResourceManager::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
