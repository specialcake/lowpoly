//
// Created by dydxh on 10/23/18.
//

#ifndef BREAKOUT_SPRITERENDER_H
#define BREAKOUT_SPRITERENDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Resource/texture.h"
#include "../Resource/shader.h"
#include "../Resource/resourcemanager.h"

class SpriteRenderer {
public:
    Shader shader;
    GLuint quadVAO;
    SpriteRenderer(Shader shader);
    ~SpriteRenderer();

    void DrawSprite(Texture2D &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1.0, 1.0, 1.0), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void initRenderData();
};

#endif //BREAKOUT_SPRITERENDER_H
