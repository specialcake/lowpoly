//
// Created by dydxh on 12/9/18.
//

#ifndef TMPER_GAUSSBLUR_H
#define TMPER_GAUSSBLUR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Resource/shader.h"
#include "../Resource/resourcemanager.h"

class Gaussblur {
public:
    static Shader shaderBlur;
    static GLuint pingpongFBO[2];
    static GLuint pingpongBuffer[2];
    static void Initialize(const Shader& shader);
    static GLuint GaussBlur(const Texture2D& texture);
    static GLuint GaussBlur(const GLuint& textureID);
private:
    static GLuint VAO;
    static void initRenderData();
    static void RenderQuad();

};


#endif //TMPER_GAUSSBLUR_H
