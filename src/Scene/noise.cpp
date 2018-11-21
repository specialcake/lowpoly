//
// Created by dydxh on 11/17/18.
//

#include <iostream>
#include "noise.h"
#include "../utils/tools.h"
#include "../config.h"

Noise::Noise(GLint size) : size(size) {
    P = new GLint[size];
    G = new glm::vec2[size];
    for(GLint i = 0; i < size; i++) {
        P[i] = i;
        if(i != 0) Tools::swap(P[i], P[rand() % i]);
        GLfloat theta = Tools::random(0, 2 * PI);
        G[i] = glm::vec2(cos(theta), sin(theta));
    }
}
Noise::~Noise() {
    delete[] P;
    delete[] G;
}

GLfloat Noise::Generate(glm::vec3 loc) {
    glm::vec2 p = glm::vec2(loc.x, loc.z);
//    Tools::PrintVec2(p);
//    printf("====>");
    GLint i;
    if(p.x < 0){
        for(i = 1; p.x + i * size < 0; i <<= 1);
        for(;i > 0; i >>= 1){
            if(p.x + i * size < size)
                p.x += i * size;
        }
    }
    else if(p.x >= size){
        for(i = 1; p.x - i * size > 0; i <<= 1);
        for(;i > 0; i >>= 1){
            if(p.x - i * size > 0)
                p.x -= i * size;
        }
    }
    if(p.y < 0){
        for(i = 1; p.y + i * size < 0; i <<= 1);
        for(;i > 0; i >>= 1){
            if(p.y + i * size < size)
                p.y += i * size;
        }
    }
    else if(p.y >= size){
        for(i = 1; p.y - i * size > 0; i <<= 1);
        for(;i > 0; i >>= 1){
            if(p.y - i * size > 0)
                p.y -= i * size;
        }
    }
//    Tools::PrintVec2(p);
//    GLfloat ret = default_noise(p);
//    printf("height => %.3lf\n", ret);
//    return ret;
    return default_noise(p);

//    for(GLint i = 0; i < width; i++){
//        for(GLint j = 0; j < height; j++){
//            noise[i][j] = default_noise(glm::vec2(i + Tools::random01(), j + Tools::random01()));
////            std::cout << noise[i][j] << ' ';
//        }
////        std::cout << std::endl;
//    }
}
GLfloat Noise::default_noise(glm::vec2 p) {
    return perlin_noise(p);
}

GLfloat Noise::perlin_noise(glm::vec2 p) {
    glm::vec2 pi = glm::floor(p);
    glm::vec2 pf = p - pi;
    glm::vec2 w = pf * pf * pf * (6.0f * pf * pf - 15.0f * pf + 10.0f);

    return glm::mix(glm::mix(glm::dot(get_vector(pi + glm::vec2(0.0f, 0.0f)), pf - glm::vec2(0.0f, 0.0f)),
                             glm::dot(get_vector(pi + glm::vec2(1.0f, 0.0f)), pf - glm::vec2(1.0f, 0.0f)), w.x),
                    glm::mix(glm::dot(get_vector(pi + glm::vec2(0.0f, 1.0f)), pf - glm::vec2(0.0f, 1.0f)),
                             glm::dot(get_vector(pi + glm::vec2(1.0f, 1.0f)), pf - glm::vec2(1.0f, 1.0f)), w.x),
                    w.y);
}

glm::vec2 Noise::get_vector(glm::vec2 p) {
    GLint x = static_cast<GLint>(p.x), y = static_cast<GLint>(p.y);
    return G[(x + P[y]) % size];
}

//void Noise::DebugInitial(Shader shader) {
//    this->DebugShader = shader;
//
//    this->vertices = {
//            // Pos      // Color
//            1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//            0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//
//            0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
//            1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//            0.0f, 0.0f, 1.0f, 1.0f, 1.0f
//    };
//
//    glGenVertexArrays(1, &this->DebugVAO);
//    glGenBuffers(1, &VBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
//
//    glBindVertexArray(this->DebugVAO);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//}
//
//void Noise::DebugRender() {
//    this->DebugShader.use();
//
//    for(GLint i = 0; i < this->width - 1; i++){
//        for(GLint j = 0; j < this->height - 1; j++){
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(1.0f * i / (this->width - 1), 0.0f, 1.0f * j / (this->height - 1)));
//            model = glm::scale(model, glm::vec3(1.0f / (this->width - 1), 1.0f, 1.0f / (this->height - 1)));
//
//            this->DebugShader.setMat4("model", model);
//
//            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//            void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//            for(GLint k = 0; k < 6; k++){
//                auto x = (GLint)this->vertices[k * 5 + 0], y = (GLint)this->vertices[k * 5 + 1];
//                this->vertices[k * 5 + 2] = (this->noise[i + x][j + y] * 0.5f + 0.5f) * 25.0f / 255.0f;
//                this->vertices[k * 5 + 3] = (this->noise[i + x][j + y] * 0.5f + 0.5f) * 161.0f / 255.0f;
//                this->vertices[k * 5 + 4] = (this->noise[i + x][j + y] * 0.5f + 0.5f) * 245.0f / 255.0f;
//            }
//            memcpy(ptr, &this->vertices[0], sizeof(GLfloat) * this->vertices.size());
//            glUnmapBuffer(GL_ARRAY_BUFFER);
//
//            glBindVertexArray(this->DebugVAO);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            glBindVertexArray(0);
//        }
//    }
//}