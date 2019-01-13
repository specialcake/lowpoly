#include "game.h"
#include "utils/spriterender.h"
#include "utils/cuberender.h"
#include "Scene/scene.h"
#include "Postprocess/shadowmap.h"
#include "Postprocess/gaussblur.h"
#include "Postprocess/skymap.h"
#include "Postprocess/postprocessor.h"
#include "Postprocess/Bloom.h"
#include "Polyball/polyball.h"
#include "Polyball/startplatform.h"
#include "Polyball/finishline.h"

Scene* scene;
SpriteRenderer* littlewindow;
CubeRender* littlecube;
Shadowmap* shadowmap;
Skymap* skymap;
PostProcessor* SceneTexture;
Polyball* polyball;
StartPlatform* startplatform;
FinishLine* finishline;
//SSR* SSReflect;
//Terrian* plane;

Game::Game(GLuint width, GLuint height) : Width(width), Height(height) {
    ResourceManager::lastX = width / 2.0f;
    ResourceManager::lastY = height / 2.0f;
}

Game::~Game() {}

void Game::Init() {
    ResourceManager::LoadShader("../src/shader/scene.vert", "../src/shader/scene.frag", NULL, "scene");
    ResourceManager::LoadShader("../src/shader/sprite.vert", "../src/shader/sprite.frag", NULL, "sprite");
    ResourceManager::LoadShader("../src/shader/cuberender.vert", "../src/shader/cuberender.frag", NULL, "cuberender");
    ResourceManager::LoadShader("../src/shader/instancescene.vert", "../src/shader/instancescene.frag", "../src/shader/instancescene.geom", "instancescene");
//    ResourceManager::LoadShader("../src/shader/ssrscene.vert", "../src/shader/ssrscene.frag", "../src/shader/ssrscene.geom", "ssrscene");
//    ResourceManager::LoadShader("../src/shader/SSR.vert", "../src/shader/SSR.frag", NULL, "SSR");
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
    ResourceManager::LoadShader("../src/shader/polyball.vert", "../src/shader/polyball.frag", "../src/shader/polyball.geom", "polyball");
    ResourceManager::LoadShader("../src/shader/fontdisplay.vert", "../src/shader/fontdisplay.frag", NULL, "fontdisplay");
    ResourceManager::LoadShader("../src/shader/startplatform.vert", "../src/shader/startplatform.frag", "../src/shader/startplatform.geom", "startplatform");

    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setInt("image", 0);

    ResourceManager::GetShader("gaussblur").use();
    ResourceManager::GetShader("gaussblur").setInt("image", 0);

    ResourceManager::GetShader("model").use();
    ResourceManager::GetShader("model").setInt("BlurShadow", 0);

    ResourceManager::GetShader("bloom").use();
    ResourceManager::GetShader("bloom").setInt("scene", 0);
    ResourceManager::GetShader("bloom").setInt("bloomBlur", 1);

    ResourceManager::GetShader("polyball").use();
    ResourceManager::GetShader("polyball").setInt("BlurShadow", 0);

    ResourceManager::GetShader("instancescene").use();
    ResourceManager::GetShader("instancescene").setInt("HeightMap", 0);
    ResourceManager::GetShader("instancescene").setInt("DepthMap", 4);
    ResourceManager::GetShader("instancescene").setInt("BlurShadow", 5);

//    ResourceManager::GetShader("ssrscene").use();
//    ResourceManager::GetShader("ssrscene").setInt("HeightMap", 0);
//    ResourceManager::GetShader("ssrscene").setInt("BlurShadow", 1);

//    ResourceManager::GetShader("SSR").use();
//    ResourceManager::GetShader("SSR").setInt("DepthMap", 0);
//    ResourceManager::GetShader("SSR").setInt("NormalMap", 1);
//    ResourceManager::GetShader("SSR").setInt("ColorMap", 2);
//    ResourceManager::GetShader("SSR").setInt("Reflectable", 3);

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

    ResourceManager::GetShader("fontdisplay").use();
    ResourceManager::GetShader("fontdisplay").setInt("text", 0);

    ResourceManager::GetShader("startplatform").use();
    ResourceManager::GetShader("startplatform").setInt("BlurShadow", 0);

    ResourceManager::LoadModel("../resource/model/tree/pine.obj", "pine");
    ResourceManager::LoadModel("../resource/model/tree/normaltree.obj", "normaltree");
    ResourceManager::LoadModel("../resource/model/polyball/polyball.obj", "polyball", false);
//    ResourceManager::LoadModel("../resource/model/rock/bigrock.obj", "bigrock");
    ResourceManager::LoadModel("../resource/model/rock/stone2.obj", "smallrock", false);
    ResourceManager::LoadModel("../resource/model/startplatform.obj", "startplatform");
    ResourceManager::LoadModel("../resource/model/finishline.obj", "finishline");
    ResourceManager::GetModel("pine")->SetBias(0.0f, -0.9f, 0.0f);
//    ResourceManager::GetModel("smallrock")->SetBias(0.0f, -0.1f, 0.0f);

    ResourceManager::fontdisplay.Initialize(ResourceManager::GetShader("fontdisplay"));

    littlewindow = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    littlecube = new CubeRender(ResourceManager::GetShader("cuberender"));

    scene = new Scene(glm::vec3(0.0f), ResourceManager::GetShader("scene"));
    scene->map_instance_shader = ResourceManager::GetShader("instancescene");
    scene->water_shader = ResourceManager::GetShader("water");
    scene->shadow_shader = ResourceManager::GetShader("shadowmap");
    scene->generate_scene();
    for(int i = 0; i < TREENUMBER; i++) {
        scene->plant[i]->shader = ResourceManager::GetShader("instancemodel");
        scene->plant[i]->shadowshader = ResourceManager::GetShader("modelshadow");
    }

    shadowmap = new Shadowmap();
    shadowmap->Blurer.Initialize(ResourceManager::GetShader("gaussblur"));

    ResourceManager::skybox = new Skybox(ResourceManager::GetShader("skybox"));

    skymap = new Skymap(ResourceManager::GetShader("skymap"));
    skymap->BeginMakeMap();
    skymap->shader.use();
    skymap->Draw(glm::vec3(0.0f, 0.5f, -1.0f));
    skymap->EndMakeMap();

    SceneTexture = new PostProcessor();
    Bloom::Initialize(ResourceManager::GetShader("bloom"));
    Bloom::Blurer.Initialize(ResourceManager::GetShader("gaussblur"));

    ResourceManager::StartLocation  = scene->FindStartFinishLocation(0);
    ResourceManager::FinishLocation = scene->FindStartFinishLocation(1);
    printf("Start Position: ");
    Tools::PrintVec3(ResourceManager::StartLocation);
    printf("Finish Position: ");
    Tools::PrintVec3(ResourceManager::FinishLocation);
    polyball = new Polyball(ResourceManager::GetShader("polyball"), ResourceManager::StartLocation + glm::vec3(0.0f, 0.36f, 0.0f));
    startplatform = new StartPlatform(ResourceManager::GetModel("startplatform"),
                                      ResourceManager::GetShader("startplatform"),
                                      ResourceManager::StartLocation);
    finishline = new FinishLine(ResourceManager::GetModel("finishline"),
                                ResourceManager::GetShader("model"),
                                ResourceManager::FinishLocation);

//    SSReflect = new SSR(ResourceManager::GetShader("ssrscene"),
//                        ResourceManager::GetShader("ssrscene"),
//                        ResourceManager::GetShader("SSR"));

//    ResourceManager::skybox->LoadTexture(skymap->skymap);
}

void Game::Update(GLfloat dt) {
    static int first_time = 1;
    static GLfloat accumulate_time = 0.0f;
    accumulate_time += dt;

    scene->UpdateChunks(polyball->Position);

//    scene->generate_cloud(accumulate_time);
//    scene->CloudMap = scene->Generate_CloudMap();

    if(ResourceManager::dir != ORIGIN_POS || first_time){
        scene->HeightMap = scene->Generate_HeightMap();
        scene->CloudMap = scene->Generate_CloudMap();
        scene->Generate_Treeplace();
        for(int i = 0; i < TREENUMBER; i++) {
            scene->plant[i]->SetParam(scene->Treeplace[i]);
        }

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

        shadowmap->BluredShadow = shadowmap->Blurer.GaussBlur(shadowmap->DepthMap);
    }

    first_time = 0;
}

void Game::ProcessInput(GLfloat dt) {
    static GLint start_signal = 0;
    static GLfloat accumulate_time = 0.0f;
    if (ResourceManager::Keys[GLFW_KEY_UP])
        ResourceManager::camera.ProcessKeyboard(FORWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_DOWN])
        ResourceManager::camera.ProcessKeyboard(BACKWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_LEFT])
        ResourceManager::camera.ProcessKeyboard(LEFT, dt);
    if (ResourceManager::Keys[GLFW_KEY_RIGHT])
        ResourceManager::camera.ProcessKeyboard(RIGHT, dt);
    if (ResourceManager::Keys[GLFW_KEY_Q])
        ResourceManager::camera.ProcessKeyboard(ROLL_LEFT, dt);
    if (ResourceManager::Keys[GLFW_KEY_E])
        ResourceManager::camera.ProcessKeyboard(ROLL_RIGHT, dt);
    if (ResourceManager::Keys[GLFW_KEY_F])
        ResourceManager::followMode ^= 1;
    if(ResourceManager::Keys[GLFW_KEY_SPACE])
        start_signal = 1;
    if(start_signal && (accumulate_time < 2.0f || !ResourceManager::active_signal)){
        ResourceManager::State = GAME_STARTING;
        accumulate_time += dt;
    }
    if(ResourceManager::State != GAME_ACTIVE && ResourceManager::State != GAME_FINISH && accumulate_time >= 2.0f && ResourceManager::active_signal)
        ResourceManager::State = GAME_ACTIVE;
    polyball->Cam.Front = ResourceManager::camera.Front;
    polyball->Cam.Right = ResourceManager::camera.Right;
    polyball->Cam.Up = ResourceManager::camera.Up;

    polyball->UpdateSpeed(dt);
    polyball->UpdatePosition(dt, scene);
//    polyball->UpdateSpeed(0.017f);
//    polyball->UpdatePosition(0.017f, scene);
    if(ResourceManager::followMode)
        ResourceManager::camera.SetPosition(polyball->GenCameraPosition());
}

void Game::Render() {
    glm::mat4 projection = glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
#ifdef lightview
    projection = glm::ortho(-40.0f, 40.0f, -12.0f, 14.0f, 0.1f, 150.0f);
#endif //lightview
    glm::mat4 view = ResourceManager::camera.GetViewMatrix();
    glm::mat4 PVMatrix = projection * view;
    glm::mat4 lightSpaceMatrix = shadowmap->GetlightSpaceMatrix(scene);

//        Model* test = ResourceManager::GetModel("finishline");
//        Shader modelshader = ResourceManager::GetShader("model");
//        modelshader.use();
//        glm::mat4 model = glm::mat4(1.0f);
////        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
//        model = glm::scale(model, glm::vec3(0.3));
//        modelshader.setMat4("model", model);
//        modelshader.setMat4("PVMatrix", PVMatrix);
//        test->Draw(modelshader);

    SceneTexture->BeginRender();

    finishline->Render(PVMatrix, lightSpaceMatrix, shadowmap->BluredShadow);
    if(ResourceManager::State == GAME_INITIAL || ResourceManager::State == GAME_STARTING)
        startplatform->Render(PVMatrix, lightSpaceMatrix, shadowmap->BluredShadow);

    scene->draw(view, PVMatrix, lightSpaceMatrix, shadowmap->DepthMap, shadowmap->BluredShadow);
    for(int i = 0; i < 3; i++)
        scene->plant[i]->Draw(view, PVMatrix, lightSpaceMatrix, shadowmap->BluredShadow);

    glDepthFunc(GL_LEQUAL);
    ResourceManager::skybox->shader.use();
    glActiveTexture(GL_TEXTURE6);
    scene->CloudMap.Bind();
    ResourceManager::skybox->shader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
    ResourceManager::skybox->Draw(skymap->skymap);
    glActiveTexture(GL_TEXTURE0);

    Model* Sun = ResourceManager::GetModel("polyball");
    Shader sunshader = ResourceManager::GetShader("sun");
    sunshader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, -8.0f * PARLIGHT_DIR);
    model = glm::scale(model, glm::vec3(40.0f));
    sunshader.setMat4("model", model);
    sunshader.setMat4("PVMatrix", projection * glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix())));
    sunshader.setVec3("lightdir", PARLIGHT_DIR);

    Sun->Draw(sunshader);

    glDepthFunc(GL_LESS);

    polyball->Render(view, PVMatrix, lightSpaceMatrix, shadowmap->BluredShadow);

    littlecube->DrawCube(PVMatrix, glm::vec3(0.0f, 0.2f, 3.0f), glm::vec3(0.05f));

    SceneTexture->EndRender();

    Bloom::RenderBloom(SceneTexture);
//
//    Texture2D blurscene = Bloom::Blurer.GaussBlur(SceneTexture->BrightTexture);
//
//        littlewindow->shader.use();
//        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
//        littlewindow->DrawSprite(SceneTexture->ColorTexture, glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f));

//        littlewindow->shader.use();
//        littlewindow->shader.setMat4("PVMatrix", glm::mat4(1.0f));
//        littlewindow->DrawSprite(blurscene, glm::vec3(-0.1f, 0.5f, -0.5f), glm::vec3(0.5f));
//    std::string text = "Location = (" + std::to_string(polyball->Position.x) + ',' +
//                                        std::to_string(polyball->Position.y) + ',' +
//                                        std::to_string(polyball->Position.z) + ')';
//    ResourceManager::Displayfont(text, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
//    text = "Speed = (" + std::to_string(polyball->Speed.x) + ',' +
//                         std::to_string(polyball->Speed.y) + ',' +
//                         std::to_string(polyball->Speed.z) + ')';
//    ResourceManager::Displayfont(text, glm::vec3(0.0f, 35.0f, 0.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
//    text = "Chunk-Mesh = (" + std::to_string(polyball->dcx) + ',' +
//                       std::to_string(polyball->dcy) + "),(" +
//                       std::to_string(polyball->dmx) + ',' +
//                       std::to_string(polyball->dmy) + ')';
//    ResourceManager::Displayfont(text, glm::vec3(0.0f, 70.0f, 0.0f), glm::vec3(0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
}
