//
// Created by dydxh on 11/19/18.
//

#ifndef TMPER_SCENE_H
#define TMPER_SCENE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "chunk.h"
#include "submesh.h"
#include "../config.h"
#include "../Resource/shader.h"
#include "noise.h"
#include <iostream>
#include <vector>

class Scene {
public:
    Chunk* chunk[CHUNK_SIZE][CHUNK_SIZE];
    Chunk* cur_Chunk;
    Submesh* cur_Submesh;
    Chunk* tmp_chunk[3];

    Noise* generator;

    glm::vec3 offset;
    glm::vec3 chunk_offset[CHUNK_SIZE][CHUNK_SIZE];
    glm::vec3 mesh_offset[MESH_SIZE + 1][MESH_SIZE + 1];

    Shader map_shader, water_shader;
    GLuint VAO, VBO;
    std::vector<GLfloat> vertices;
    Scene(glm::vec3 initpos, Shader shader);
    ~Scene();
    void Initialize();
    void InitBuffer();
    void draw();
    void generate_scene();

    void UpdateChunks();
    void GetLocationbyCamera(GLint& cx, GLint& cz, GLint& ms, GLint& mz);
private:

};


#endif //TMPER_SCENE_H
