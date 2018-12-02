//
// Created by dydxh on 11/17/18.
//

#include <iostream>
#include <cassert>
#include "noise.h"
#include "../utils/tools.h"
#include "../config.h"

Noise::Noise(GLint size) : size(size) {
    P = new GLint[size];
    G = new glm::vec2[size];
//    for(GLint i = 0; i < size; i++) {
//        P[i] = i;
//        if(i != 0) Tools::swap(P[i], P[rand() % i]);
//        GLfloat theta = Tools::random(0, 2 * PI);
//        G[i] = glm::vec2(cos(theta), sin(theta));
//    }
    P[0] = 2; P[1] = 3; P[2] = 15; P[3] = 26; P[4] = 11;
    P[5] = 14; P[6] = 4; P[7] = 19; P[8] = 1; P[9] = 22;
    P[10] = 27; P[11] = 23; P[12] = 6; P[13] = 12; P[14] = 18;
    P[15] = 17; P[16] = 5; P[17] = 10; P[18] = 9; P[19] = 24;
    P[20] = 7; P[21] = 20; P[22] = 13; P[23] = 25; P[24] = 0;
    P[25] = 28; P[26] = 21; P[27] = 16; P[28] = 8;
    G[0] = glm::vec2(0.684770, -0.728759); G[1] = glm::vec2(0.031835, -0.999493); G[2] = glm::vec2(-0.999705, 0.024282); G[3] = glm::vec2(-0.165815, 0.986157);
    G[4] = glm::vec2(-0.576616, 0.817016); G[5] = glm::vec2(0.989431, 0.145007); G[6] = glm::vec2(0.994057, -0.108861); G[7] = glm::vec2(-0.684243, 0.729254);
    G[8] = glm::vec2(0.499701, 0.866198); G[9] = glm::vec2(0.644026, -0.765004); G[10] = glm::vec2(-0.615800, 0.787902); G[11] = glm::vec2(0.926926, 0.375244);
    G[12] = glm::vec2(0.904772, 0.425897); G[13] = glm::vec2(0.528370, 0.849014); G[14] = glm::vec2(-0.996860, -0.079180); G[15] = glm::vec2(-0.999941, -0.010838);
    G[16] = glm::vec2(0.088838, -0.996046); G[17] = glm::vec2(0.716457, 0.697631); G[18] = glm::vec2(-0.991936, -0.126737); G[19] = glm::vec2(0.829506, 0.558497);
    G[20] = glm::vec2(-0.226003, 0.974127); G[21] = glm::vec2(0.980895, 0.194538); G[22] = glm::vec2(0.946064, 0.323980); G[23] = glm::vec2(-0.619802, -0.784758);
    G[24] = glm::vec2(-0.842756, 0.538296); G[25] = glm::vec2(-0.950774, 0.309886); G[26] = glm::vec2(-0.925944, -0.377660); G[27] = glm::vec2(-0.562101, 0.827068);
    G[28] = glm::vec2(0.957367, -0.288874);
//    for(GLint i = 0; i < size; i++) {
//        printf("P[%d] = %d; ", i, P[i]);
//        if(i % 5 == 4) printf("\n");
//    }
//    printf("\n");
//    for(GLint i = 0; i < size; i++) {
//        printf("G[%d] = glm::vec2(%f, %f); ", i, G[i].x, G[i].y);
//        if(i % 4 == 3) printf("\n");
//    }
//    printf("\n");
}
Noise::~Noise() {
    delete[] P;
    delete[] G;
}

glm::vec2 Noise::processer(glm::vec2 p) {

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
    return p;
}

GLfloat Noise::Generate(glm::vec3 loc) {
    glm::vec2 p = glm::vec2(loc.x, loc.z);
    p *= 0.1f;
//    Tools::PrintVec2(p);
    GLfloat ret = default_noise(p);
//    printf("height => %.3lf\n", ret);
    ret = ret * 0.5f + 0.25f;
    ret *= 3.0f;
    if(ret < 0.0f) ret = 0.0f;
    ret = pow(ret, 2.23);
    return ret;
}
GLfloat Noise::default_noise(glm::vec2 p) {
//    static bool debug_flag = true;
    GLfloat ret = 0.0f;
//    printf("%f\n", ret);
    GLfloat A = 1.0f;

    for(GLint i = 0; i < 5; i++){
        p = processer(p);
        ret += A * perlin_noise(p);
//        if(debug_flag == true){
//            Tools::PrintVec2(p);
//            printf("==> %f\n", ret);
//        }
        p = 2.0f * p; A = 0.5f * A;
    }
//    debug_flag = false;
//    ret += 1.0000f * perlin_noise(p); p = 2.0f * p;
//    printf("%f\n", ret);
//    ret += 0.5000f * perlin_noise(p); p = 2.0f * p;
//    printf("%f\n", ret);
//    ret += 0.2500f * perlin_noise(p); p = 2.0f * p;
//    printf("%f\n", ret);
//    ret += 0.1250f * perlin_noise(p); p = 2.0f * p;
//    printf("%f\n", ret);
//    ret += 0.0625f * perlin_noise(p); p = 2.0f * p;
//    printf("%f\n", ret);
    return ret;
//    p = p * 4.0f;
    ret += 1.0000f * abs(perlin_noise(p)); p = 2.0f * p;
    ret += 0.5000f * abs(perlin_noise(p)); p = 2.0f * p;
    ret += 0.2500f * abs(perlin_noise(p)); p = 2.0f * p;
    ret += 0.1250f * abs(perlin_noise(p)); p = 2.0f * p;
    ret += 0.0625f * abs(perlin_noise(p)); p = 2.0f * p;
    return ret;
    return perlin_noise(p);
}

GLfloat Noise::perlin_noise(glm::vec2 p) {
//    static bool debug_flag = true;
    glm::vec2 pi = glm::floor(p);
    glm::vec2 pf = p - pi;
    glm::vec2 w = pf * pf * pf * (6.0f * pf * pf - 15.0f * pf + 10.0f);

//    GLfloat vala = glm::dot(get_vector(pi + glm::vec2(0.0f, 0.0f)), pf - glm::vec2(0.0f, 0.0f));
//    GLfloat valb = glm::dot(get_vector(pi + glm::vec2(1.0f, 0.0f)), pf - glm::vec2(1.0f, 0.0f));
//    GLfloat valc = glm::dot(get_vector(pi + glm::vec2(0.0f, 1.0f)), pf - glm::vec2(0.0f, 1.0f));
//    GLfloat vald = glm::dot(get_vector(pi + glm::vec2(1.0f, 1.0f)), pf - glm::vec2(1.0f, 1.0f));
//
//    if(debug_flag == true){
//        printf("======\n");
//        Tools::PrintVec4(glm::vec4(vala, valb, valc, vald));
//        printf("======\n");
//    }
//    debug_flag = false;
//
//    return glm::mix(glm::mix(vala, valb, w.x), glm::mix(valc, vald, w.x), w.y);

    return glm::mix(glm::mix(glm::dot(get_vector(pi + glm::vec2(0.0f, 0.0f)), pf - glm::vec2(0.0f, 0.0f)),
                             glm::dot(get_vector(pi + glm::vec2(1.0f, 0.0f)), pf - glm::vec2(1.0f, 0.0f)), w.x),
                    glm::mix(glm::dot(get_vector(pi + glm::vec2(0.0f, 1.0f)), pf - glm::vec2(0.0f, 1.0f)),
                             glm::dot(get_vector(pi + glm::vec2(1.0f, 1.0f)), pf - glm::vec2(1.0f, 1.0f)), w.x),
                    w.y);
}

glm::vec2 Noise::get_vector(glm::vec2 p) {
//    p = glm::vec2(dot(p, glm::vec2(127.1f, 311.7f)),
//                  dot(p, glm::vec2(269.5f, 183.3f)));
//
//    return -1.0f + 2.0f * fract(sin(p) * 43758.5453123f);
    GLint x = static_cast<GLint>(p.x), y = static_cast<GLint>(p.y);
//    assert(x >= 0 && y >= 0);
    return G[(x + P[y % size]) % size];
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