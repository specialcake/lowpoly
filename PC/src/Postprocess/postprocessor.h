//
// Created by dydxh on 10/24/18.
//

#ifndef BREAKOUT_POSTPROCESSOR_H
#define BREAKOUT_POSTPROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../Resource/texture.h"
#include "../Resource/shader.h"
#include "../utils/spriterender.h"

class PostProcessor {
public:
    Shader PostProcessingShader;
    Texture2D Texture;
    GLuint Width, Height;

    PostProcessor(Shader shader, GLuint width, GLuint height);

    void BeginRender();
    void EndRender();

    void Render();
private:
    GLuint MSFBO, FBO;
    GLuint RBO;
    GLuint VAO;

    void initRenderData();
};

#endif //BREAKOUT_POSTPROCESSOR_H
