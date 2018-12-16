//
// Created by dydxh on 12/4/18.
//

#ifndef TMPER_SHADOWMAP_H
#define TMPER_SHADOWMAP_H

#include "../Resource/resourcemanager.h"
#include "../Resource/texture.h"

class Shadowmap {
public:
    Texture2D DepthMap, BluredShadow;
    Shadowmap();
    ~Shadowmap();
    void Initialize();
    glm::mat4 GetlightSpaceMatrix();
    void BeginMakeMap();
    void EndMakeMap();
private:
    GLuint DepthMapFBO;
};


#endif //TMPER_SHADOWMAP_H
