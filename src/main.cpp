#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "config.h"
#include "Resource/resourcemanager.h"
#include "game.h"

float lastFrame = 0.0f, deltaTime = 0.0f;

Game Application(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]) {
    ResourceManager::window_initialize();

    ResourceManager::State = GAME_INITIAL;
    Application.Init();
    while (!glfwWindowShouldClose(ResourceManager::window)) {
        auto currentFrame = static_cast<GLfloat>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        Application.ProcessInput(deltaTime);
        Application.Update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        Application.Render();

        glfwSwapBuffers(ResourceManager::window);
    }
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}
