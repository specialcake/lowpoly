//
// Created by dydxh on 1/8/19.
//

#ifndef TMPER_POLYBALL_H
#define TMPER_POLYBALL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
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
    GLfloat Radius;
    Coord Mov, Cam;
    Collision collision;

    GLfloat wspeed;
    glm::vec3 rotate_axis;
    glm::quat rotate_state;

    glm::vec3 Maycol[5][5];

    Model* modelptr;
    Shader BallShader;

    GLint dcx,dcy,dmx,dmy;

    Polyball(Shader shader);
    ~Polyball();
    void Initialize();
    glm::vec3 GenCameraPosition();
    void Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow);
    glm::vec3 CollisionObject(Scene* scene);
    void CollisionCheck(Scene* scene);
    glm::vec3 CollisionFinish();
    void GetChunkMeshID(Scene* scene, GLint &cx, GLint &cz, GLint &mx, GLint &mz);
    void UpdatePosition(float deltaTime, Scene* scene);
    void UpdateSpeed(float deltaTime);
    void UpdateMovVec();
};


#endif //TMPER_POLYBALL_H
