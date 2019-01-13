//
// Created by dydxh on 12/16/18.
//

#include "water.h"
#include <string>

Water::Water() {
    Initialize();
}
Water::~Water() {

}

void Water::Initialize() {
    wave[0] = (Wave){0.05f, 0.0f, 0.6f, 2.1f, 0.0f, 0.0f};
    wave[0].D = glm::vec2(-0.468f, -0.956f);

    wave[1] = (Wave){0.02f, 0.0f, 0.4f, 3.5f, 0.0f, 0.0f};
    wave[1].D = glm::vec2(-0.111f, 0.379f);

    wave[2] = (Wave){0.07f, 0.0f, 0.5f, 4.2f, 0.0f, 0.0f};
    wave[2].D = glm::vec2(0.53f, -0.239f);

    wave[3] = (Wave){0.03f, 0.0f, 0.7f, 5.0f, 0.0f, 0.0f};
    wave[3].D = glm::vec2(0.497f, 0.78f);

    for(int i = 0; i < NUMOFWATER; i++){
        wave[i].w = static_cast<GLfloat>(sqrt(9.8 * 2 * PI / wave[i].L));
        wave[i].phi = wave[i].S * wave[i].w;
        wave[i].Q = CONSTQ / (wave[i].w * wave[i].A * NUMOFWATER);
    }
}
void Water::Setwater(Shader shader) {
    for(int i = 0; i < NUMOFWATER; i++){
        std::string name = "wave[" + std::to_string(i) + "].";
        shader.setFloat(name + "A", wave[i].A);
        shader.setFloat(name + "Q", wave[i].Q);
        shader.setFloat(name + "phi", wave[i].phi);
        shader.setFloat(name + "w", wave[i].w);
        shader.setVec2(name + "D", wave[i].D);
    }
    shader.setFloat("Timer", glfwGetTime());
//    shader.setFloat("Timer", 0.0f);
}
