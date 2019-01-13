//
// Created by dydxh on 1/8/19.
//

#ifndef TMPER_BLOOM_H
#define TMPER_BLOOM_H

#include "../Resource/resourcemanager.h"
#include "postprocessor.h"
#include "gaussblur.h"

class Bloom {
public:
    static Gaussblur Blurer;
    static Shader bloomShader;
    static void Initialize(const Shader& shader);
    static void RenderBloom(PostProcessor* SceneMap);
};


#endif //TMPER_BLOOM_H
