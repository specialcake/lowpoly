//
// Created by dydxh on 1/3/19.
//

#ifndef TMPER_PLANT_H
#define TMPER_PLANT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Resource/model.h"
#include "../config.h"
#include <vector>

struct Treeinfo{
    int chunk_number;
    GLfloat height;
    glm::vec3 location;
};

class Plants {
public:
    GLuint amount;
    glm::mat4 matrixs[MESH_SIZE * MESH_SIZE];
    Plants();
    ~Plants();
    void Initialize();
    void SetParam(const std::vector<Treeinfo>& places);
    void Draw(Shader shader);
};


#endif //TMPER_PLANT_H
