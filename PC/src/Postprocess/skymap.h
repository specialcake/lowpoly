//
// Created by dydxh on 12/22/18.
//

#ifndef TMPER_SKYMAP_H
#define TMPER_SKYMAP_H

#include <glad/glad.h>
#include "../Resource/texture.h"
#include "../Resource/shader.h"

class Skymap {
public:
    Texture2D skymap[6];
    Shader shader;
    GLuint quadVAO;
    Skymap(Shader shader);
    ~Skymap();
    void Initialize();
    void BeginMakeMap();
    void EndMakeMap();
    void InitRenderData();
    void Draw(glm::vec3 SunPos);
private:
    GLuint SkymapFBO;

};


#endif //TMPER_SKYMAP_H
