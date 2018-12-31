//
// Created by dydxh on 11/19/18.
//

#ifndef TMPER_SUBMESH_H
#define TMPER_SUBMESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../config.h"

class Chunk;

class Submesh {
public:
    GLint pos_x, pos_z;
    Meshtype MeshType;
    GLboolean* Occupied;
    Chunk* parent;
    Submesh *xPos, *xNeg, *zPos, *zNeg;
    Submesh(GLint x, GLint z);
    ~Submesh();
    void Initialize();
    void draw_map();
    void draw_water();
    glm::vec3 get_Position();

    void recycle(GLint x, GLint z);
private:
};


#endif //TMPER_SUBMESH_H
