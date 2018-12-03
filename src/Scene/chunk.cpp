//
// Created by dydxh on 11/19/18.
//

#include "chunk.h"
#include "noise.h"
#include "../utils/tools.h"
#include "scene.h"
#include <iostream>
#include <cstdio>

Chunk::Chunk(GLint x, GLint z) : pos_x(x), pos_z(z){
    this->Initialize();
}

Chunk::~Chunk() {

}
void Chunk::Initialize() {
    this->xPos = this->xNeg = this->zPos = this->zNeg = nullptr;
//    memset(this->height, 0, sizeof(this->height));
    for(GLint i = 0; i <= MESH_SIZE; i++) {
        for(GLint j = 0; j <= MESH_SIZE; j++) {
            this->submesh[i][j] = new Submesh(i, j);
            this->submesh[i][j]->parent = this;
        }
    }
}

void Chunk::recycle(GLint x, GLint z) {
    this->xPos = this->xNeg = this->zPos = this->zNeg = nullptr;
    this->pos_x = x, this->pos_z = z;
    for(int i = 0; i <= MESH_SIZE; i++){
        for(int j = 0; j <= MESH_SIZE; j++){
            this->submesh[i][j]->recycle(i, j);
        }
    }
    this->generate_height();
//    this->generate_water();
//    this->generate_normal();
}
void Chunk::Updateinfo() { //After all height generated
    this->generate_water();
    this->generate_normal();
}

void Chunk::generate_height() {
    for(GLint i = 0; i <= MESH_SIZE; i++) {
        for(GLint j = 0; j <= MESH_SIZE; j++) {
            this->height[i][j] = this->parent->generator->Generate(this->submesh[i][j]->get_Position());
            this->submesh[i][j]->MeshType = LAND;
        }
    }
}
void Chunk::generate_water() {
    for(GLint i = 0; i < MESH_SIZE; i++) {
        for(GLint j = 0; j < MESH_SIZE; j++) {
            if(this->height[i][j] <= SEA_LEVEL
               || this->height[i][j + 1] <= SEA_LEVEL
               || this->height[i + 1][j] <= SEA_LEVEL
               || this->height[i + 1][j + 1] <= SEA_LEVEL)
                this->submesh[i][j]->MeshType = WATER;
//            printf("(%d, %d) ", i, j);
        }
    }
//    printf("\n");
}
void Chunk::generate_normal() {
    for(GLint i = 0; i < MESH_SIZE; i++) {
        for(GLint j = 0; j < MESH_SIZE; j++) {
            glm::vec3 v1 = glm::vec3(0.0f, height[i][j + 1] - height[i][j], MESH_LENGTH);
            glm::vec3 v2 = glm::vec3(MESH_LENGTH, height[i + 1][j] - height[i][j], 0.0f);
            normal[0][i][j] = glm::normalize(glm::cross(v1, v2));

            v1 = glm::vec3(0.0f, height[i + 1][j] - height[i + 1][j + 1], -MESH_LENGTH);
            v2 = glm::vec3(-MESH_LENGTH, height[i][j + 1] - height[i + 1][j + 1], 0.0f);
            normal[1][i][j] = glm::normalize(glm::cross(v1, v2));

            pnormal[i][j] = calc_normal(i, j);
        }
    }
}
glm::vec3 Chunk::calc_normal(GLint x, GLint z) {  //point normal calc
    GLfloat center_height = height[x][z];
    GLfloat zNeg_height = -100.0f, zPos_height = -100.0f, xNeg_height = -100.0f, xPos_height = -100.0f;
    if(z > 0) zNeg_height = height[x][z - 1];
    else if(pos_z > 0) zNeg_height = zNeg->height[x][MESH_SIZE - 1];
    if(z < MESH_SIZE - 1) zPos_height = height[x][z + 1];
    else if(pos_z < CHUNK_SIZE - 1) zPos_height = zPos->height[x][0];
    if(x > 0) xNeg_height = height[x - 1][z];
    else if(pos_x > 0) xNeg_height = xNeg->height[MESH_SIZE - 1][z];
    if(x < MESH_SIZE - 1) xPos_height = height[x + 1][z];
    else if(pos_x < CHUNK_SIZE - 1) xPos_height = xPos->height[0][z];

    if(zNeg_height > -50.0f) zNeg_height -= center_height;
    if(zPos_height > -50.0f) zPos_height -= center_height;
    if(xNeg_height > -50.0f) xNeg_height -= center_height;
    if(xPos_height > -50.0f) xPos_height -= center_height;

    glm::vec3 v1, v2, v3, v4;
    v1 = v2 = v3 = v4 = glm::vec3(0.0f);
    if(zNeg_height > -50.0f) v1 = glm::vec3(0.0f, zNeg_height, -MESH_LENGTH);
    if(zPos_height > -50.0f) v3 = glm::vec3(0.0f, zPos_height, MESH_LENGTH);
    if(xNeg_height > -50.0f) v4 = glm::vec3(-MESH_LENGTH, xNeg_height, 0.0f);
    if(xPos_height > -50.0f) v2 = glm::vec3(MESH_LENGTH, xPos_height, 0.0f);

    glm::vec3 ret = glm::vec3(0.0f);
    ret = glm::cross(v2, v1) +
          glm::cross(v3, v2) +
          glm::cross(v4, v3) +
          glm::cross(v1, v4);
    ret = glm::normalize(ret);
    return ret;
}

void Chunk::draw_map() {
    for(GLint i = 0; i < MESH_SIZE; i++) {
        for(GLint j = 0; j < MESH_SIZE; j++) {
            this->submesh[i][j]->draw_map();
        }
    }
}

void Chunk::draw_water() {
    int a = 0;
    for(GLint i = 0; i < MESH_SIZE; i++) {
        for(GLint j = 0; j < MESH_SIZE; j++) {
            if(this->submesh[i][j]->MeshType == WATER) {
                this->submesh[i][j]->draw_water();
                a++;
            }
        }
    }
//    std::cout << a << std::endl;
}