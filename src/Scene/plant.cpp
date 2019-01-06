//
// Created by dydxh on 1/3/19.
//

#include "plant.h"
#include "../Resource/resourcemanager.h"

Plants::Plants(std::string modelname) {
    Initialize(modelname);
}

Plants::~Plants() {}

void Plants::Initialize(std::string modelname) {
    modelptr = ResourceManager::GetModel(modelname);
}

bool operator < (const Treeinfo& x, const Treeinfo& y){
    return x.chunk_number < y.chunk_number;
}

void Plants::SetParam(const std::vector<Treeinfo>& places) {
    this->amount = places.size();

    for(int i = 0; i < amount; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-modelptr->cx, -modelptr->miny - 1.0f + places[i].height, -modelptr->cz));
        model = glm::translate(model, places[i].location);
        model = glm::scale(model, glm::vec3(0.3f));
        matrixs[i] = model;
    }

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matrixs, GL_STATIC_DRAW);

    for(int i = 0; i < modelptr->meshes.size(); i++){
        glBindVertexArray(modelptr->meshes[i].VAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)sizeof(glm::vec4));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void Plants::Draw(const glm::mat4& view, const glm::mat4& PVMatrix, const glm::mat4& lightSpaceMatrix, const Texture2D& BluredShadow) {
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    BluredShadow.Bind();
    shader.setMat4("view", view);
    shader.setMat4("PVMatrix", PVMatrix);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setLight(ResourceManager::camera.Position);
    for(int i = 0; i < modelptr->meshes.size(); i++){
        shader.setVec4("texture_diffuse1_color", modelptr->meshes[i].color);
        glBindVertexArray(modelptr->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, modelptr->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}
void Plants::GenerateShadow(const glm::mat4 &lightSpaceMatrix) {
    shadowshader.use();
    shadowshader.setMat4("PVMatrix", lightSpaceMatrix);
    for(int i = 0; i < modelptr->meshes.size(); i++){
        glBindVertexArray(modelptr->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, modelptr->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}
