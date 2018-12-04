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
    bool debugflag;
    Chunk* chunk[CHUNK_SIZE][CHUNK_SIZE];
    Chunk* cur_Chunk;
    Submesh* cur_Submesh;
    Chunk* tmp_chunk[3];

    glm::vec3 offset;
    glm::vec3 chunk_offset[CHUNK_SIZE][CHUNK_SIZE];
    glm::vec3 mesh_offset[MESH_SIZE + 1][MESH_SIZE + 1];

    Noise* generator;
    Texture2D HeightMap;
    Texture2D NormalMap0, NormalMap1, pNormalMap;

    Shader map_shader, map_instance_shader, water_shader;
    GLuint VAO, VBO, instanceVAO, instanceVBO;
    std::vector<GLfloat> vertices;

    Scene(glm::vec3 initpos, Shader shader);
    ~Scene();
    void Initialize();
    void InitBuffer();
    void draw(const glm::mat4& PVMatrix, const glm::mat4& lightSpaceMatrix, const Texture2D& ShadowMap);
    void generate_scene();

    void UpdateChunks();
    void UpdateNeighbor(GLint x, GLint y);
    void GetLocationbyCamera(GLint& cx, GLint& cz, GLint& ms, GLint& mz);

    Texture2D Generate_HeightMap();
    Texture2D Generate_NormalMap(int th);
    Texture2D Generate_pNormalMap();
private:

};


#endif //TMPER_SCENE_H
