//
// Created by dydxh on 1/6/19.
//

#ifndef TMPER_SSR_H
#define TMPER_SSR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/resourcemanager.h"

class SSR {
public:
    GLuint VAO;
    Texture2D DepthMap, NormalMap, ColorMap;
    Shader SceneShader, ModelShader, SSRShader;
    SSR(Shader scene_shader, Shader model_shader, Shader ssr_shader);
    ~SSR();
    void Initialize();
    void InitRenderData();
    void BeginMakeUp();
    void EndMakeUp();
    void Render();
private:
    GLuint SSRFBO;
};


#endif //TMPER_SSR_H
