#include "game.h"
#include "Resource/resourcemanager.h"
#include "utils/spriterender.h"
#include "Scene/scene.h"
#include "Postprocess/shadowmap.h"
#include "Postprocess/gaussblur.h"
#include "Postprocess/skymap.h"
#include "Postprocess/postprocessor.h"
#include "Postprocess/Bloom.h"

Scene* scene;
SpriteRenderer* littlewindow;
Shadowmap* shadowmap;
Skymap* skymap;
PostProcessor* SceneTexture;
//SSR* SSReflect;
//Terrian* plane;

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Width(width), Height(height) {
    ResourceManager::lastX = width / 2.0f;
    ResourceManager::lastY = height / 2.0f;
}

Game::~Game() {}

void Game::Init() {
    ResourceManager::LoadShader("../src/shader/scene.vert", "../src/shader/scene.frag", NULL, "scene");
    ResourceManager::LoadShader("../src/shader/sprite.vert", "../src/shader/sprite.frag", NULL, "sprite");
    ResourceManager::LoadShader("../src/shader/instancescene.vert", "../src/shader/instancescene.frag", "../src/shader/instancescene.geom", "instancescene");
    ResourceManager::LoadShader("../src/shader/ssrscene.vert", "../src/shader/ssrscene.frag", "../src/shader/ssrscene.geom", "ssrscene");
    ResourceManager::LoadShader("../src/shader/SSR.vert", "../src/shader/SSR.frag", NULL, "SSR");
    ResourceManager::LoadShader("../src/shader/instancemodel.vert", "../src/shader/instancemodel.frag", NULL, "instancemodel");
    ResourceManager::LoadShader("../src/shader/water.vert", "../src/shader/water.frag", "../src/shader/water.geom", "water");
    ResourceManager::LoadShader("../src/shader/shadowmap.vert", "../src/shader/shadowmap.frag", "../src/shader/shadowmap.geom", "shadowmap");
    ResourceManager::LoadShader("../src/shader/modelshadow.vert", "../src/shader/modelshadow.frag", NULL, "modelshadow");
    ResourceManager::LoadShader("../src/shader/model.vert", "../src/shader/model.frag", NULL, "model");
    ResourceManager::LoadShader("../src/shader/sun.vert", "../src/shader/sun.frag", NULL, "sun");
    ResourceManager::LoadShader("../src/shader/gaussblur.vert", "../src/shader/gaussblur.frag", NULL, "gaussblur");
    ResourceManager::LoadShader("../src/shader/skybox.vert", "../src/shader/skybox.frag", NULL, "skybox");
    ResourceManager::LoadShader("../src/shader/skymap.vert", "../src/shader/skymap.frag", NULL, "skymap");
    ResourceManager::LoadShader("../src/shader/bloom.vert", "../src/shader/bloom.frag", NULL, "bloom");

    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setInt("image", 0);

    ResourceManager::GetShader("gaussblur").use();
    ResourceManager::GetShader("gaussblur").setInt("image", 0);

    ResourceManager::GetShader("bloom").use();
    ResourceManager::GetShader("bloom").setInt("scene", 0);
    ResourceManager::GetShader("bloom").setInt("bloomBlur", 1);

    ResourceManager::GetShader("instancescene").use();
    ResourceManager::GetShader("instancescene").setInt("HeightMap", 0);
    ResourceManager::GetShader("instancescene").setInt("DepthMap", 4);
    ResourceManager::GetShader("instancescene").setInt("BlurShadow", 5);

    ResourceManager::GetShader("ssrscene").use();
    ResourceManager::GetShader("ssrscene").setInt("HeightMap", 0);
    ResourceManager::GetShader("ssrscene").setInt("BlurShadow", 1);

    ResourceManager::GetShader("SSR").use();
    ResourceManager::GetShader("SSR").setInt("DepthMap", 0);
    ResourceManager::GetShader("SSR").setInt("NormalMap", 1);
    ResourceManager::GetShader("SSR").setInt("ColorMap", 2);
    ResourceManager::GetShader("SSR").setInt("Reflectable", 3);

    ResourceManager::GetShader("instancemodel").use();
    ResourceManager::GetShader("instancemodel").setInt("BlurShadow", 0);

    ResourceManager::GetShader("water").use();
    ResourceManager::GetShader("water").setInt("ShadowMap", 0);
    ResourceManager::GetShader("water").setInt("BlurShadow", 1);

    ResourceManager::GetShader("shadowmap").use();
    ResourceManager::GetShader("shadowmap").setInt("HeightMap", 0);

    ResourceManager::GetShader("skybox").use();
    ResourceManager::GetShader("skybox").setInt("Right", 0);
    ResourceManager::GetShader("skybox").setInt("Left", 1);
    ResourceManager::GetShader("skybox").setInt("Top", 2);
    ResourceManager::GetShader("skybox").setInt("Bottom", 3);
    ResourceManager::GetShader("skybox").setInt("Front", 4);
    ResourceManager::GetShader("skybox").setInt("Back", 5);
    ResourceManager::GetShader("skybox").setInt("Cloud", 6);

    ResourceManager::LoadModel("../resource/model/tree/pine.obj", "pine");
    ResourceManager::LoadModel("../resource/model/tree/normaltree.obj", "normaltree");
    ResourceManager::LoadModel("../resource/model/polyball/polyball.obj", "polyball");
    ResourceManager::LoadModel("../resource/model/rock/bigrock.obj", "bigrock");
    ResourceManager::GetModel("pine")->SetBias(4.0f, -0.8f, 27.0f);
    ResourceManager::GetModel("normaltree")->SetBias(-9.0f, -6.8f, 38.0f);
    ResourceManager::GetModel("bigrock")->SetBias(-1.0f, 1.5f, 2.7f);

    littlewindow = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    scene = new Scene(glm::vec3(0.0f), ResourceManager::GetShader("scene"));
    scene->map_instance_shader = ResourceManager::GetShader("instancescene");
    scene->water_shader = ResourceManager::GetShader("water");
    scene->shadow_shader = ResourceManager::GetShader("shadowmap");
    scene->generate_scene();
    scene->Generate_Treeplace();
    for(int i = 0; i < TREENUMBER; i++){
        std::cout << "QAQ[" << i << "] => " << scene->Treeplace[i].size() << std::endl;
        scene->plant[i]->shader = ResourceManager::GetShader("instancemodel");
        scene->plant[i]->shadowshader = ResourceManager::GetShader("modelshadow");
    }

    shadowmap = new Shadowmap();
    Gaussblur::Initialize(ResourceManager::GetShader("gaussblur"));

    ResourceManager::skybox = new Skybox(ResourceManager::GetShader("skybox"));

    skymap = new Skymap(ResourceManager::GetShader("skymap"));
    skymap->BeginMakeMap();
    skymap->shader.use();
    skymap->Draw(glm::vec3(0.0f, 0.5f, -1.0f));
    skymap->EndMakeMap();

    SceneTexture = new PostProcessor();
    Bloom::Initialize(ResourceManager::GetShader("bloom"));

//    SSReflect = new SSR(ResourceManager::GetShader("ssrscene"),
//                        ResourceManager::GetShader("ssrscene"),
//                        ResourceManager::GetShader("SSR"));

//    ResourceManager::skybox->LoadTexture(skymap->skymap);
}

void Game::Update(GLfloat dt) {
    static int first_time = 1;
    static GLfloat accumulate_time = 0.0f;
    accumulate_time += dt;

    scene->UpdateChunks();

//    scene->generate_cloud(accumulate_time);
//    scene->CloudMap = scene->Generate_CloudMap();

    if(ResourceManager::dir != ORIGIN_POS || first_time){
        scene->HeightMap = scene->Generate_HeightMap();
        scene->CloudMap = scene->Generate_CloudMap();
        for(int i = 0; i < TREENUMBER; i++)
            scene->plant[i]->SetParam(scene->Treeplace[i]);

//        scene->UpdateTreeplace();

        shadowmap->UpdateFrustum(scene);

        glm::mat4 lightSpaceMatrix = shadowmap->GetlightSpaceMatrix(scene);
        glm::mat4 viewmat = glm::lookAt(PARLIGHT_POSITION + scene->offset,
                                        PARLIGHT_POSITION + scene->offset + PARLIGHT_DIR,
                                        glm::vec3(0.0f, 0.0f, 0.0f));
        shadowmap->BeginMakeMap();
        scene->Generate_ShadowMap(lightSpaceMatrix, viewmat);
        for(int i = 0; i < TREENUMBER; i++)
            scene->plant[i]->GenerateShadow(lightSpaceMatrix);
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
        glm::mat4 projection = glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
#ifdef lightview
        projection = glm::ortho(-40.0f, 40.0f, -12.0f, 14.0f, 0.1f, 150.0f);
#endif //lightview
        glm::mat4 view = ResourceManager::camera.GetViewMatrix();
        glm::mat4 PVMatrix = projection * view;
        glm::mat4 lightSpaceMatrix = shadowmap->GetlightSpaceMatrix(scene);

        SceneTexture->BeginRender();

        glDepthFunc(GL_LEQUAL);
        ResourceManager::skybox->shader.use();
        glActiveTexture(GL_TEXTURE6);
        scene->CloudMap.Bind();
        ResourceManager::skybox->shader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
        ResourceManager::skybox->Draw(skymap->skymap);
        glDepthFunc(GL_LESS);

        scene->draw(view, PVMatrix, lightSpaceMatrix, shadowmap->DepthMap, shadowmap->BluredShadow);
        for(int i = 0; i < TREENUMBER; i++)
            scene->plant[i]->Draw(view, PVMatrix, lightSpaceMatrix, shadowmap->BluredShadow);

//        Model* test = ResourceManager::GetModel("bigrock");
//        Shader modelshader = ResourceManager::GetShader("model");
//        modelshader.use();
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, test->BiasVector());
//        model = glm::scale(model, glm::vec3(0.05f));
//        modelshader.setMat4("model", model);
//        modelshader.setMat4("PVMatrix", PVMatrix);
//        test->Draw(modelshader);

//        SSReflect->Render(projection);

        glDepthFunc(GL_LEQUAL);
        Model* Sun = ResourceManager::GetModel("polyball");
        Shader sunshader = ResourceManager::GetShader("sun");
        sunshader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, -8.0f * PARLIGHT_DIR);
        model = glm::scale(model, glm::vec3(25.0f));
        sunshader.setMat4("model", model);
        sunshader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
        sunshader.setVec3("lightdir", PARLIGHT_DIR);

        Sun->Draw(sunshader);
        glDepthFunc(GL_LESS);

//        glDepthFunc(GL_LEQUAL);
//        ResourceManager::skybox->shader.use();
//        glActiveTexture(GL_TEXTURE6);
//        scene->CloudMap.Bind();
//        ResourceManager::skybox->shader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
//        ResourceManager::skybox->Draw(skymap->skymap);
//        glDepthFunc(GL_LESS);

        SceneTexture->EndRender();

        littlewindow->shader.use();
        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
        littlewindow->DrawSprite(SceneTexture->ColorTexture, glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(2.0f));

        littlewindow->shader.use();
        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
        littlewindow->DrawSprite(SceneTexture->BrightTexture, glm::vec3(-0.1f, 0.5f, -0.5f), glm::vec3(0.5f));
    }
}
