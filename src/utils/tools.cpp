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
GLfloat Tools::fmod(GLfloat a, GLfloat m) {
    return a - (int)(a / m) * m;
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
//p to segment(a, b)
GLfloat Tools::PointToSegment(glm::vec3 p, glm::vec3 a, glm::vec3 b) {
    GLfloat cross = glm::dot(p - a, b - a);
    if(cross <= 0) return glm::length(p - a);
    GLfloat ABlen = glm::dot(b - a, b - a);
    if(cross >= ABlen) return glm::length(p - b);
    glm::vec3 Q = cross / ABlen * (b - a);
    return glm::length(p - Q);
}
GLint Tools::sign(GLfloat val) {
    return val > 0 ? 1 : val < 0 ? -1 : 0;
}
//if p in triangle(a,b,c), p, a, b, c are in same plane
bool Tools::inTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 sign1 = glm::cross(a - p, b - p);
    glm::vec3 sign2 = glm::cross(b - p, c - p);
    glm::vec3 sign3 = glm::cross(c - p, a - p);
    return glm::dot(sign1, sign2) >= 0 && glm::dot(sign2, sign3) >= 0;
}
//point p to triangle(a,b,c)
GLfloat Tools::distance(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c){
    GLfloat V = glm::abs(glm::dot(glm::cross(a - p, b - p), c - p)) / 6.0;
    GLfloat S = glm::length(glm::cross(a - c, b - c)) / 2.0;
    GLfloat h = V / S * 3.0;
    glm::vec3 Normal = glm::normalize(glm::cross(c - a, b - a));
    glm::vec3 Q = p - Normal * h;
    if(inTriangle(Q, a, b, c)) return h;
    GLfloat len1 = PointToSegment(Q, a, b);
    GLfloat len2 = PointToSegment(Q, b, c);
    GLfloat len3 = PointToSegment(Q, c, a);
    GLfloat minlen = glm::min(len1, glm::min(len2, len3));
    return glm::sqrt(minlen * minlen + h * h);
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