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
    Gaussblur(){};
    Shader shaderBlur;
    GLuint pingpongFBO[2];
    GLuint pingpongBuffer[2];
    void Initialize(const Shader& shader);
    GLuint GaussBlur(const Texture2D& texture);
private:
    GLuint VAO;
    void initRenderData();
    void RenderQuad();

};


#endif //TMPER_GAUSSBLUR_H
