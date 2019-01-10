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
struct Coord{
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
};

class Polyball{
public:
    glm::vec3 Speed, SpeedLimit;
    glm::vec3 Acceleration, Resistance;
    glm::vec3 Position;
    Coord Mov, Cam;
    Collision collision;

    Model* modelptr;
    Shader BallShader;

    Polyball(Shader shader);
    ~Polyball();
    void Initialize();
    glm::vec3 GenCameraPosition();
    void Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow);
    void UpdatePosition(float deltaTime);
    void UpdateSpeed(float deltaTime);
    void UpdateMovVec();
};


#endif //TMPER_POLYBALL_H
