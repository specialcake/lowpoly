//
// Created by dydxh on 1/8/19.
//

#ifndef TMPER_POLYBALL_H
#define TMPER_POLYBALL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/resourcemanager.h"
#include "../Resource/camera.h"
#include "../Scene/scene.h"
#include "../config.h"
#include "../utils/tools.h"

struct Collision{
    bool exist;
    glm::vec3 Point;
    glm::vec3 Normal;
};

class Polyball{
public:
    GLfloat Speed;
    glm::vec3 movdir, Position;
    glm::vec3 Front, Up, Right;
    Collision collision;

    Model* modelptr;
    Shader BallShader;

    Polyball(Shader shader);
    ~Polyball();
    void Initialize();
    glm::vec3 GenCameraPosition();
    void Update();
    void Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow);
};


#endif //TMPER_POLYBALL_H
