//
// Created by dydxh on 1/8/19.
//

#include "polyball.h"

Polyball::Polyball(Shader shader) {
    Speed = 0.0f;
    movdir = glm::vec3(0.0f);
    BallShader = shader;
    Position = glm::vec3(0.0f, 3.0f, 2.0f);
    collision = (Collision){false, glm::vec3(0.0f), glm::vec3(0.0f)};
    Front = ResourceManager::camera.Front;
    Right = ResourceManager::camera.Right;
    Up = ResourceManager::camera.Up;
    Initialize();
}

Polyball::~Polyball() {}

void Polyball::Initialize() {
    modelptr = ResourceManager::GetModel("polyball");
}

glm::vec3 Polyball::GenCameraPosition() {
    const GLfloat distance = 3.0f;
    static GLfloat cosvalue = glm::cos(PI / 12.0);
    static GLfloat sinvalue = glm::sin(PI / 12.0);
    Front = ResourceManager::camera.Front;
    Right = ResourceManager::camera.Right;
    Up = ResourceManager::camera.Up;
    glm::vec3 ret = Position;
    ret += -distance * cosvalue * Front;
    ret += distance * sinvalue * Up;
    return ret;
}

void Polyball::Update() {

}

void Polyball::Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
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
