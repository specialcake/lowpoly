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
    for(GLint i = 0; i <= MESH_SIZE; i++) {
        for(GLint j = 0; j <= MESH_SIZE; j++) {
            this->submesh[i][j] = new Submesh(i, j);
            this->submesh[i][j]->parent = this;
//            if(i + 1 < MESH_SIZE)
//                this->submesh[i][j]->xPos = this->submesh[i + 1][j];
//            if(i != 0)
//                this->submesh[i][j]->xNeg = this->submesh[i - 1][j];
//            if(j + 1 < MESH_SIZE)
//                this->submesh[i][j]->zPos = this->submesh[i][j + 1];
//            if(j != 0)
//                this->submesh[i][j]->zNeg = this->submesh[i][j - 1];
        }
    }
}

void Chunk::recycle(GLint x, GLint z) {
    this->xPos = this->xNeg = this->zPos = this->zNeg = nullptr;
    this->pos_x = x, this->pos_z = z;
    for(int i = 0; i <= MESH_SIZE; i++){
        for(int j = 0; j <= MESH_SIZE; j++){
            this->submesh[i][j]->recycle(i, j);
            this->height[i][j] = this->parent->generator->Generate(this->submesh[i][j]->get_Position());
//            this->height[i][j] = (this->height[i][j] * 0.5f + 0.25f) * MAX_HEIGHT;
            this->submesh[i][j]->MeshType = LAND;
        }
    }
    this->generate_water();
}

void Chunk::generate_map() {
    static bool debugflag = false;
    memset(this->height, 0, sizeof(this->height));
//    if(this->pos_x != 4 || this->pos_z != 0) return ;
    for(GLint i = 0; i <= MESH_SIZE; i++) {
        for(GLint j = 0; j <= MESH_SIZE; j++) {
            this->height[i][j] = this->parent->generator->Generate(this->submesh[i][j]->get_Position());
//            this->height[i][j] = (this->height[i][j] * 0.5f + 0.25f) * MAX_HEIGHT;
            this->submesh[i][j]->MeshType = LAND;
        }
    }
    if(debugflag){
        printf("(%d, %d):\n", pos_x, pos_z);
        for(GLint i = 0; i <= MESH_SIZE; i++) {
            for(GLint j = 0; j <= MESH_SIZE; j++) {
                printf("%.3lf ", this->height[i][j]);
            }
            printf("\n");
        }
        printf("===========================\n");
//        debugflag = false;
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