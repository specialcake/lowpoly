//
// Created by dydxh on 11/17/18.
//

#ifndef TMPER_NOISE_H
#define TMPER_NOISE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/texture.h"
#include "../Resource/shader.h"
#include <vector>

class Noise {
public:
    Noise(GLint size);
    ~Noise();

    GLfloat Generate(glm::vec3 loc);
    GLfloat CloudGenerate(glm::vec3 loc);
//    std::vector<GLfloat> vertices;
//    GLuint DebugVAO, VBO;
//    Shader DebugShader;
//    void DebugInitial(Shader shader);
//    void DebugRender();
private:
    GLint size;
    glm::vec2* G;
    GLint* P;

    glm::vec2 processer(glm::vec2 p);
    GLfloat default_noise(glm::vec2 p);
    GLfloat cloud_noise(glm::vec2 p);
    GLfloat perlin_noise(glm::vec2 p);
    glm::vec2 get_vector(glm::vec2 p);
};


#endif //TMPER_NOISE_H
