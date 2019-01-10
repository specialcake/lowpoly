//
// Created by dydxh on 1/8/19.
//

#include "polyball.h"

Polyball::Polyball(Shader shader) {
    Speed = glm::vec3(0.0f, 0.0f, 0.0f);
    SpeedLimit = glm::vec3(0.0f);
    Acceleration = glm::vec3(2.0f, 0.0f, 2.0f);
    Resistance = glm::vec3(1.0f, 3.0f, 1.0f);

    Position = glm::vec3(0.0f, 0.01f, 2.0f);
    Mov.Front = glm::vec3(1.0f, 0.0f, 0.0f);
    Mov.Right = glm::vec3(0.0f, 0.0f, 1.0f);
    Mov.Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    Cam.Front = ResourceManager::camera.Front;
    Cam.Right = ResourceManager::camera.Right;
    Cam.Up    = ResourceManager::camera.Up;

    if(Position.y > 0.0f)
        collision = (Collision){false, glm::vec3(0.0f), glm::vec3(0.0f)};
    else
        collision = (Collision){true,  glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};

    BallShader = shader;
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
    glm::vec3 ret = Position;
    ret += -distance * cosvalue * Cam.Front;
    ret += distance * sinvalue * Cam.Up;
    return ret;
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

//    Tools::PrintVec3(Position);
//    Tools::PrintVec3(Speed);
}

//UpdatePosition => Process Speed
void Polyball::UpdatePosition(float deltaTime) {
    collision = (Collision){false, glm::vec3(0.0f), glm::vec3(0.0f)};
    Position += Speed * deltaTime;
    if(Position.y <= 0.0f){
        Position.y = 0.0f;
        collision = (Collision){true, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
    }
}

//UpdateSpeed => Process Acceleration
void Polyball::UpdateSpeed(float deltaTime) {
    if(collision.exist){
        GLfloat velocity = -glm::dot(collision.Normal, Speed);
        Speed += velocity * collision.Normal;
        if(velocity > 2.0f)
            Speed += 0.1f * velocity * collision.Normal;
    }
    UpdateMovVec();

    glm::vec3 delta_v = glm::vec3(0.0f);
    bool noKeyboard = true;
    if(collision.exist){
        if(ResourceManager::Keys[GLFW_KEY_W]){
            GLfloat cosvalue = glm::dot(Cam.Front, Mov.Up);
            delta_v += (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_S]){
            GLfloat cosvalue = glm::dot(Cam.Front, Mov.Up);
            delta_v -= (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_D]){
            GLfloat cosvalue = glm::dot(Cam.Right, Mov.Up);
            delta_v += (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_A]){
            GLfloat cosvalue = glm::dot(Cam.Right, Mov.Up);
            delta_v -= (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_SPACE]){
            delta_v += glm::vec3(0.0f, 3.0f, 0.0f);
        }
        //Sliding Friction
        glm::vec3 movXZ = Speed;
        GLfloat movSpeed = glm::length(movXZ);
        if(movSpeed > eps) {
            if (Resistance.x * deltaTime > movSpeed && noKeyboard)
                delta_v -= glm::normalize(movXZ) * movSpeed;
            else
                delta_v -= glm::normalize(movXZ) * Resistance.x * deltaTime;
        }
    }
    //Gravity
    delta_v.y -= Resistance.y * deltaTime;
    Speed += delta_v;

    UpdateMovVec();
}
void Polyball::UpdateMovVec(){
    if(glm::abs(Speed.x) < eps && glm::abs(Speed.z) < eps){
        Mov.Front = glm::vec3(1.0f, 0.0f, 0.0f);
        Mov.Right = glm::vec3(0.0f, 0.0f, 1.0f);
        Mov.Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else{
        Mov.Front = glm::normalize(Speed);
        Mov.Right = glm::normalize(glm::cross(Mov.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Mov.Up    = glm::normalize(glm::cross(Mov.Right, Mov.Front));
    }
}
