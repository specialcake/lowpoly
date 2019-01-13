//
// Created by dydxh on 1/13/19.
//

#include "finishline.h"

FinishLine::FinishLine(Model *model, Shader shader) {
    PartNumber = model->meshes.size();
    modelptr = model;
    this->shader = shader;
}

FinishLine::~FinishLine() {

}

void FinishLine::Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow) {
    shader.use();
    shader.setMat4("PVMatrix", PVMatrix);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setLight(ResourceManager::camera.Position);
    glActiveTexture(GL_TEXTURE0);
    BluredShadow.Bind();
    for(int i = 0; i < PartNumber; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        shader.setMat4("model", model);
        modelptr->meshes[i].Draw(shader);
    }
}
