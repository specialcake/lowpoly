//
// Created by dydxh on 1/13/19.
//

#ifndef TMPER_FINISHLINE_H
#define TMPER_FINISHLINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/resourcemanager.h"
#include "../Resource/model.h"
#include "../Scene/scene.h"
#include <map>
#include <vector>

class FinishLine {
public:
    int PartNumber;
    Model* modelptr;
    Shader shader;
    glm::vec3 offset;
    glm::vec3 center[110];

    FinishLine(Model* model, Shader shader, glm::vec3 location = glm::vec3(0.0f, 0.0f, 3.0f));
    ~FinishLine();
    void Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow);
};


#endif //TMPER_FINISHLINE_H
