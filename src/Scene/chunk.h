//
// Created by dydxh on 11/19/18.
//

#ifndef TMPER_CHUNK_H
#define TMPER_CHUNK_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "submesh.h"
#include "../config.h"

class Scene;

class Chunk {
public:
    GLint pos_x, pos_z;
    Submesh* submesh[MESH_SIZE + 1][MESH_SIZE + 1];
    GLfloat height[MESH_SIZE + 1][MESH_SIZE + 1];
    glm::vec3 normal[2][MESH_SIZE][MESH_SIZE];
    Scene* parent;
    Chunk *xPos, *xNeg, *zPos, *zNeg;
    Chunk(GLint x, GLint z);
    ~Chunk();
    void Initialize();
    void generate_map();
    void generate_water();
    void generate_mountain();
    void generate_tree();
    void generate_normal();
    void draw();
    void draw_map();
    void draw_water();
    void draw_mountain();
    void draw_tree();

    void recycle(GLint x, GLint z);
private:
};


#endif //TMPER_CHUNK_H
