//
// Created by dydxh on 1/8/19.
//

#ifndef TMPER_POLYBALL_H
#define TMPER_POLYBALL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/resourcemanager.h"
#include "../Scene/scene.h"

class Polyball {
public:
    glm::vec3 pos, v;

    Model* modelptr;
    Shader BallShader;

    Polyball(Shader shader, glm::vec3 initial_position = glm::vec3(0.0f, 3.0f, 2.0f));
    ~Polyball();
    void Initialize();
    void Update();
    void Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow);
};


#endif //TMPER_POLYBALL_H
