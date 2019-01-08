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
    Texture2D ColorTexture, BrightTexture, DepthTexture;
    GLuint Width, Height;
    GLuint VAO, FBO;
    PostProcessor();

    void Initialize();

    void BeginRender();
    void EndRender();
private:
    void initRenderData();
};

#endif //BREAKOUT_POSTPROCESSOR_H
