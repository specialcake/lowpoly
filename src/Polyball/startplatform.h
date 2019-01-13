//
// Created by dydxh on 1/13/19.
//

#ifndef TMPER_STARTPLATFORM_H
#define TMPER_STARTPLATFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/resourcemanager.h"
#include "../Resource/model.h"
#include "../Scene/scene.h"
#include <map>
#include <vector>

class StartPlatform {
public:
    int PartNumber;
    Model* modelptr;
    Shader shader;
    std::string groupName[7];
    glm::vec3 axis[7];
    GLfloat limit[7], w[7];
    bool stop[7];
    glm::vec3 offset, center;

    StartPlatform(Model* model, Shader shader, glm::vec3 location = glm::vec3(0.0f, 0.0f, 2.0f));
    ~StartPlatform();
    void Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow);
};


#endif //TMPER_STARTPLATFORM_H
