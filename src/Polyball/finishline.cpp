//
// Created by dydxh on 1/13/19.
//

#include "finishline.h"

FinishLine::FinishLine(Model *model, Shader shader, glm::vec3 location) {
    PartNumber = model->meshes.size();
    modelptr = model;
    this->shader = shader;
    offset = location;
    for(int i = 2; i < PartNumber; i++)
        center[i] = model->meshes[i].GetCenter();
}

FinishLine::~FinishLine() {

}

void FinishLine::Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow) {
    shader.use();
    shader.setVec3("offset", offset);
    shader.setMat4("PVMatrix", PVMatrix);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setLight(ResourceManager::camera.Position);
    glActiveTexture(GL_TEXTURE0);
    BluredShadow.Bind();
    for(int i = 0; i < 2; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        shader.setMat4("model", model);
        modelptr->meshes[i].Draw(shader);
    }
    if(ResourceManager::State != GAME_FINISH) {
        for (int i = 2; i < PartNumber; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.3f));
            shader.setMat4("model", model);
            modelptr->meshes[i].Draw(shader);
        }
    }
    else {
        static GLfloat curTime = (GLfloat)glfwGetTime();
        glm::vec3 Point = ResourceManager::FinishPos - ResourceManager::FinishLocation;
        glm::vec3 normal = ResourceManager::FinishNormal;
        for (int i = 2; i < PartNumber; i++) {
            glm::vec3 dir = center[i] - Point;
            GLfloat dist = glm::length(dir) * 5.0f;
//        dir = glm::normalize(dir);
            dir = glm::normalize(-normal * glm::length(dir) + dir);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, (GLfloat) ((glfwGetTime() - curTime) / 10.0f) * dir / dist);
            model = glm::scale(model, glm::vec3(0.3f));
            shader.setMat4("model", model);
            modelptr->meshes[i].Draw(shader);
        }
    }
}
