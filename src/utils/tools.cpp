//
// Created by dydxh on 11/12/18.
//

#include "tools.h"
#include "../config.h"
#include <cstdio>
#include <cmath>

GLfloat Tools::random01(){
    return rand() % 100000 / 99999.0f;
}
GLfloat Tools::random(GLfloat l, GLfloat r) {
    return l + (r - l) * random01();
}
GLfloat Tools::random_11(){
    return random01() * 2 - 1.0f;
}
GLfloat Tools::random_angle() {
    return random01() * PI;
}
glm::vec2 Tools::random_vec2() {
    return glm::normalize(glm::vec2(random01(), random01()));
}
glm::vec3 Tools::random_vec3() {
    return glm::normalize(glm::vec3(random01(), random01(), random01()));
}
GLfloat Tools::distance(glm::vec2 a, glm::vec2 b) {
    return sqrt((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y));
}
GLfloat Tools::distance(glm::vec3 a, glm::vec3 b) {
    return sqrt((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y) +
                (a.z - b.z) * (a.z - b.z));
}

void Tools::PrintVec2(glm::vec2 val){
    for(int i = 0; i < 2; i++){
        printf("%.3lf ", val[i]);
    }
    printf("\n");
}
void Tools::PrintVec3(glm::vec3 val){
    for(int i = 0; i < 3; i++){
        printf("%.3lf ", val[i]);
    }
    printf("\n");
}
void Tools::PrintVec4(glm::vec4 val){
    for(int i = 0; i < 4; i++){
        printf("%.3lf ", val[i]);
    }
    printf("\n");
}
void Tools::PrintMat3(glm::mat3 val){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("%.3lf ", val[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void Tools::PrintMat4(glm::mat4 val){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%.3lf ", val[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}