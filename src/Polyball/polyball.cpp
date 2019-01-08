//
// Created by dydxh on 1/8/19.
//

#include "polyball.h"

Polyball::Polyball(Shader shader, glm::vec3 initial_position) : BallShader(shader), pos(initial_position), v(glm::vec3(0.0f)) {
    Initialize();
}

Polyball::~Polyball() {

}

void Polyball::Initialize() {
    modelptr = ResourceManager::GetModel("polyball");
}

void Polyball::Update() {

}

void Polyball::Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(0.3f));

    BallShader.use();
    BallShader.setMat4("view", view);
    BallShader.setMat4("PVMatrix", PVMatrix);
    BallShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    BallShader.setMat4("model", model);
    BallShader.setLight(ResourceManager::camera.Position);

    glActiveTexture(GL_TEXTURE0);
    BlurShadow.Bind();

    modelptr->Draw(BallShader);
}
