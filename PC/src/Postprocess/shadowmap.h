//
// Created by dydxh on 12/4/18.
//

#ifndef TMPER_SHADOWMAP_H
#define TMPER_SHADOWMAP_H

#include "../Resource/resourcemanager.h"
#include "../Resource/texture.h"
#include "../Scene/scene.h"
#include "gaussblur.h"

class Shadowmap {
public:
    Texture2D DepthMap, BluredShadow;
    Gaussblur Blurer;
    Shadowmap();
    ~Shadowmap();
    void Initialize();
    glm::mat4 GetlightSpaceMatrix(const Scene* scene);
    void BeginMakeMap();
    void EndMakeMap();
    void UpdateFrustum(const Scene* scene);
private:
    GLfloat near, far, left, right, top, bottom;
    GLuint DepthMapFBO;
};


#endif //TMPER_SHADOWMAP_H
