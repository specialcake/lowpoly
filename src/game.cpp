#include "game.h"
#include "Resource/resourcemanager.h"
#include "utils/spriterender.h"
#include "Scene/scene.h"
#include "Postprocess/shadowmap.h"
#include "Postprocess/gaussblur.h"

Scene* scene;
SpriteRenderer* littlewindow;
Shadowmap* shadowmap;
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
    ResourceManager::LoadShader("../src/shader/instancescene.vert", "../src/shader/instancescene.frag", "../src/shader/instancescene.geom", "instancescene");
    ResourceManager::LoadShader("../src/shader/water.vert", "../src/shader/water.frag", "../src/shader/water.geom", "water");
    ResourceManager::LoadShader("../src/shader/shadowmap.vert", "../src/shader/shadowmap.frag", "../src/shader/shadowmap.geom", "shadowmap");
    ResourceManager::LoadShader("../src/shader/model.vert", "../src/shader/model.frag", NULL, "model");
    ResourceManager::LoadShader("../src/shader/gaussblur.vert", "../src/shader/gaussblur.frag", NULL, "gaussblur");
    ResourceManager::LoadShader("../src/shader/skybox.vert", "../src/shader/skybox.frag", NULL, "skybox");

    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setInt("image", 0);

    ResourceManager::GetShader("gaussblur").use();
    ResourceManager::GetShader("gaussblur").setInt("image", 0);

    ResourceManager::GetShader("instancescene").use();
    ResourceManager::GetShader("instancescene").setInt("HeightMap", 0);
    ResourceManager::GetShader("instancescene").setInt("DepthMap", 4);
    ResourceManager::GetShader("instancescene").setInt("BlurShadow", 5);

    ResourceManager::GetShader("water").use();
    ResourceManager::GetShader("water").setInt("ShadowMap", 0);
    ResourceManager::GetShader("water").setInt("BlurShadow", 1);

    ResourceManager::GetShader("shadowmap").use();
    ResourceManager::GetShader("shadowmap").setInt("HeightMap", 0);

    ResourceManager::LoadModel("../resource/model/widetree/widetree.obj", "crystal");

    littlewindow = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    scene = new Scene(glm::vec3(0.0f), ResourceManager::GetShader("scene"));
    scene->map_instance_shader = ResourceManager::GetShader("instancescene");
    scene->water_shader = ResourceManager::GetShader("water");
    scene->shadow_shader = ResourceManager::GetShader("shadowmap");
    scene->generate_scene();

    shadowmap = new Shadowmap();
    Gaussblur::Initialize(ResourceManager::GetShader("gaussblur"));

    ResourceManager::skybox = new Skybox(ResourceManager::GetShader("skybox"));
}

void Game::Update(GLfloat dt) {
    static int first_time = 1;

    scene->UpdateChunks();

    if(ResourceManager::dir != ORIGIN_POS || first_time){
        scene->HeightMap = scene->Generate_HeightMap();

        shadowmap->UpdateFrustum(scene);

        glm::mat4 lightSpaceMatrix = shadowmap->GetlightSpaceMatrix(scene);
        glm::mat4 viewmat = glm::lookAt(PARLIGHT_POSITION + scene->offset,
                                        PARLIGHT_POSITION + scene->offset + PARLIGHT_DIR,
                                        glm::vec3(0.0f, 0.0f, 0.0f));
        shadowmap->BeginMakeMap();
        scene->Generate_ShadowMap(lightSpaceMatrix, viewmat);
        shadowmap->EndMakeMap();

        shadowmap->BluredShadow = Gaussblur::GaussBlur(shadowmap->DepthMap);
    }

    first_time = 0;
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
        glm::mat4 projection = glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)Width / (float)Height, 0.1f, 200.0f);
#ifdef lightview
        projection = glm::ortho(-40.0f, 40.0f, -12.0f, 14.0f, 0.1f, 150.0f);
#endif //lightview
        glm::mat4 view = ResourceManager::camera.GetViewMatrix();
        glm::mat4 PVMatrix = projection * view;
        glm::mat4 lightSpaceMatrix = shadowmap->GetlightSpaceMatrix(scene);

        glDepthFunc(GL_LEQUAL);

        ResourceManager::skybox->shader.use();
        ResourceManager::skybox->shader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
        ResourceManager::skybox->Draw();

        glDepthFunc(GL_LESS);
//        littlewindow->shader.use();
//        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
//        littlewindow->DrawSprite(shadowmap->DepthMap, glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f));
//
//        littlewindow->shader.use();
//        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
//        littlewindow->DrawSprite(shadowmap->BluredShadow, glm::vec3(-0.1f, 0.5f, 0.0f), glm::vec3(0.5f));
//
        scene->draw(PVMatrix, lightSpaceMatrix, shadowmap->DepthMap, shadowmap->BluredShadow);

//        Model* mymodel = ResourceManager::GetModel("crystal");
//        Shader model_shader = ResourceManager::GetShader("model");
//        model_shader.use();
//        model_shader.setMat4("PVMatrix", PVMatrix);
//        model_shader.setLight(ResourceManager::camera.Position);
//
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(-mymodel->cx, -mymodel->cy + 4.0f, -mymodel->cz - 14.0f));
//        model = glm::scale(model, glm::vec3(0.3f));
//        model_shader.setMat4("model", model);
//        ResourceManager::GetModel("crystal")->Draw(model_shader);

        //ResourceManager::Displayfont("This is a test", glm::vec3(25.0f, 830.0f, 0.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
}
