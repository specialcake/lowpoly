//
// Created by dydxh on 1/3/19.
//

#include "plant.h"
#include "../Resource/resourcemanager.h"

Plants::Plants() {
    Initialize();
}

Plants::~Plants() {}

void Plants::Initialize() {

}

bool operator < (const Treeinfo& x, const Treeinfo& y){
    return x.chunk_number < y.chunk_number;
}

void Plants::SetParam(const std::vector<Treeinfo>& places) {
    Model* tmptr = ResourceManager::GetModel("crystal");
    this->amount = places.size();

    for(int i = 0; i < amount; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-tmptr->cx, -tmptr->cy + 4.0f + places[i].height, -tmptr->cz));
        model = glm::translate(model, places[i].location);
        model = glm::scale(model, glm::vec3(0.3f));
        matrixs[i] = model;
    }

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matrixs, GL_STATIC_DRAW);

    for(int i = 0; i < tmptr->meshes.size(); i++){
        glBindVertexArray(tmptr->meshes[i].VAO);
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

void Plants::Draw(Shader shader) {
    Model* tmptr = ResourceManager::GetModel("crystal");
    shader.setLight(ResourceManager::camera.Position);
    for(int i = 0; i < tmptr->meshes.size(); i++){
        shader.setVec4("texture_diffuse1_color", tmptr->meshes[i].color);
        glBindVertexArray(tmptr->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, tmptr->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}