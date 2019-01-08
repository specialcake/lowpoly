//
// Created by dydxh on 1/8/19.
//

#ifndef TMPER_BLOOM_H
#define TMPER_BLOOM_H

#include "../Resource/resourcemanager.h"

class Bloom {
public:
    static GLuint FBO;
    static Shader bloomShader;
    static GLuint ColorBuffer[2];
    static void Initialize(const Shader& shader);
    static void RenderBloom();
    static void BeginMakeMap();
    static void EndMakeMap();
private:
    static GLuint VAO;
    static void initRenderData();
};


#endif //TMPER_BLOOM_H
