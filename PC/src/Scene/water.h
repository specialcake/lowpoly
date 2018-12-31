//
// Created by dydxh on 12/16/18.
//

#ifndef TMPER_WATER_H
#define TMPER_WATER_H

#include <glad/glad.h>
#include "../Resource/texture.h"
#include "../Resource/shader.h"
#include "../config.h"

struct Wave{
    //Amplitude, Q, Speed, Length, phi, omega
    //Direction
    GLfloat A, Q, S, L, phi, w;
    glm::vec2 D;
};

class Water {
public:
    Wave wave[NUMOFWATER];
    Water();
    ~Water();
    void Initialize();
    void Setwater(Shader shader);
};


#endif //TMPER_WATER_H
