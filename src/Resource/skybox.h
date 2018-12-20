//
// Created by dydxh on 12/18/18.
//

#ifndef TMPER_SKYBOX_H
#define TMPER_SKYBOX_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include "texture.h"
#include "shader.h"

class Skybox {
public:
    GLuint VAO, VBO;
    Shader shader;
    Skybox(Shader shader);
    ~Skybox();
    void Initialize();
    void Draw();
private:

};


#endif //SOLARSYSTEM_SKYBOX_H
