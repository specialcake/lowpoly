//
// Created by dydxh on 1/13/19.
//

#include "startplatform.h"

StartPlatform::StartPlatform(Model* model, Shader shader, glm::vec3 location) {
    PartNumber = 7;
    modelptr = model;
    this->shader = shader;
    groupName[0] = "Base_Extension_0";
    groupName[1] = "Base_0";
    groupName[2] = "Ring_1_0";
    groupName[3] = "Ring_2_0";
    groupName[4] = "Ring_3_0";
    groupName[5] = "RIng_4_0";
    groupName[6] = "Sphere_0";
    center = glm::vec3(0.0f, 0.36f, 0.0f);
    offset = location;
    axis[2] = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f));
    axis[3] = -glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f));
    axis[4] = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
    axis[5] = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    w[2] = 1.0f;
    w[3] = 2.0f;
    w[4] = 3.0f;
    w[5] = 4.0f;
    limit[2] = glm::radians(100.0f);
    limit[3] = glm::radians(67.0f);
    limit[4] = glm::radians(26.0f);
    limit[5] = glm::radians(66.0f);
}

StartPlatform::~StartPlatform() {

}

void StartPlatform::Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow) {
    GLfloat t = glfwGetTime();
    shader.use();
    shader.setMat4("PVMatrix", PVMatrix);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shader.setLight(ResourceManager::camera.Position);
    shader.setVec3("offset", offset);
    shader.setVec3("center", center);
    glActiveTexture(GL_TEXTURE0);
    BluredShadow.Bind();

    for(int i = 0; i < 2; i++){
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setVec4("RotQuat", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        modelptr->meshes[i].Draw(shader);
    }
    shader.setMat4("model", glm::mat4(1.0f));
    static GLfloat cnt = 0.0f;
    cnt += 1.0f;
    for(int i = 2; i < PartNumber; i++){
//        shader.setVec4("RotQuat", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        GLfloat angle = glm::radians(w[i] * 40.0f * t);
        glm::quat rot = glm::quat(glm::cos(angle),
                                  glm::sin(angle) * axis[i].x,
                                  glm::sin(angle) * axis[i].y,
                                  glm::sin(angle) * axis[i].z);
        shader.setVec4("RotQuat", glm::vec4(rot.w, rot.x, rot.y, rot.z));
        modelptr->meshes[i].Draw(shader);
    }
}