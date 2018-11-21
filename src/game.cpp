#include "game.h"
#include "Resource/resourcemanager.h"
#include "utils/spriterender.h"
#include "Scene/scene.h"

Scene* scene;
SpriteRenderer* littlewindow;
//Terrian* plane;

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Width(width), Height(height) {
    ResourceManager::lastX = width / 2.0f;
    ResourceManager::lastY = height / 2.0f;
}

Game::~Game() {
}

void Game::Init() {
    ResourceManager::LoadShader("../src/shader/scene.vert", "../src/shader/scene.frag", NULL, "scene");
    ResourceManager::LoadShader("../src/shader/sprite.vert", "../src/shader/sprite.frag", NULL, "sprite");
    //ResourceManager::LoadShader("../src/shader/terrian.vert", "../src/shader/terrian.frag", NULL, "terrian");

    ResourceManager::LoadTexture("../resource/image/awesomeface.png", GL_RGBA, "awesomeface");
    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setInt("image", 0);

    littlewindow = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    scene = new Scene(glm::vec3(0.0f), ResourceManager::GetShader("scene"));

    scene->map_shader.use();

    scene->generate_scene();
    //plane = new Terrian(ResourceManager::GetShader("terrian"));
}

void Game::Update(GLfloat dt) {
    scene->UpdateChunks();
}

void Game::ProcessInput(GLfloat dt) {
    if (ResourceManager::Keys[GLFW_KEY_W])
        ResourceManager::camera.ProcessKeyboard(FORWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_S])
        ResourceManager::camera.ProcessKeyboard(BACKWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_A])
        ResourceManager::camera.ProcessKeyboard(LEFT, dt);
    if (ResourceManager::Keys[GLFW_KEY_D])
        ResourceManager::camera.ProcessKeyboard(RIGHT, dt);
    if (ResourceManager::Keys[GLFW_KEY_Q])
        ResourceManager::camera.ProcessKeyboard(ROLL_LEFT, dt);
    if (ResourceManager::Keys[GLFW_KEY_E])
        ResourceManager::camera.ProcessKeyboard(ROLL_RIGHT, dt);
}

void Game::Render() {
    if(this->State == GAME_ACTIVE) {
        glm::mat4 projection = glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
        glm::mat4 view = ResourceManager::camera.GetViewMatrix();
        glm::mat4 PVMatrix = projection * view;

        scene->map_shader.use();
        scene->map_shader.setMat4("PVMatrix", PVMatrix);
        scene->draw();

        Texture2D face = ResourceManager::GetTexture("awesomeface");
        littlewindow->shader.use();
        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
        littlewindow->DrawSprite(scene->HeightMap, glm::vec3(-1.0f), glm::vec3(0.5f));

//        plane->shader.use();
//        plane->shader.setMat4("PVMatrix", PVMatrix);
//        plane->Render();
    }
}
