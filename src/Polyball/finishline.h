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
    std::string groupName[7];

    FinishLine(Model* model, Shader shader);
    ~FinishLine();
    void Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow);
};


#endif //TMPER_FINISHLINE_H
