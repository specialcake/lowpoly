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
    heightmax[2] = 0.3;
    heightmax[3] = 0.22;
    heightmax[4] = 0.14;
    heightmax[5] = 0.1;
    stop[2] = stop[3] = stop[4] = stop[5] = false;
    vanish[2] = vanish[3] = vanish[4] = vanish[5] = false;
    drop[2] = drop[3] = drop[4] = drop[5] = false;
    limit[2] = glm::radians(100.0f);
    limit[3] = glm::radians(67.0f);
    limit[4] = glm::radians(26.0f);
    limit[5] = glm::radians(66.0f);
}

StartPlatform::~StartPlatform() {

}

void StartPlatform::Render(glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BluredShadow) {
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
    GLfloat cT = (GLfloat)glfwGetTime();
    for(int i = 2; i < PartNumber; i++){
        glm::mat4 model = glm::mat4(1.0f);
//        shader.setVec4("RotQuat", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        GLfloat angle = glm::radians(w[i] * 40.0f * cT);
        if(ResourceManager::State == GAME_STARTING) {
            if(stop[2] && stop[3] && stop[4] && stop[5]){
                static GLfloat sT = (GLfloat)glfwGetTime();
                angle = limit[i];
                if(drop[i] == true){
                    glm::vec3 delta = glm::vec3(0.0f, heightmax[i] - 2.5f * (cT - stopTime[i]) * heightmax[i], 0.0f);
                    model = glm::translate(model, delta);
                    if(delta.y < -0.2f) vanish[i] = true;
                }
                else{
                    glm::vec3 delta = glm::vec3(0.0f, 2.5f * (cT - sT) * heightmax[i], 0.0f);
                    model = glm::translate(model, delta);
                    if(delta.y >= heightmax[i]){
                        drop[i] = true;
                        stopTime[i] = cT;
                    }
                }
            }
            else{
                if(stop[i] == false){
                    if(glm::abs(Tools::fmod(angle, PI) - limit[i]) < 0.05)
                        stop[i] = true;
                }
                else{
                    angle = limit[i];
                }
            }
        }
        glm::quat rot = glm::quat(glm::cos(angle),
                        glm::sin(angle) * axis[i].x,
                        glm::sin(angle) * axis[i].y,
                        glm::sin(angle) * axis[i].z);
        shader.setMat4("model", model);
        shader.setVec4("RotQuat", glm::vec4(rot.w, rot.x, rot.y, rot.z));
        modelptr->meshes[i].Draw(shader);
    }
    if(vanish[2] && vanish[3] && vanish[4] && vanish[5])
        ResourceManager::active_signal = true;
}