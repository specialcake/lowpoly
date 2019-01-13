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
#include "water.h"
#include "plant.h"
#include <iostream>
#include <set>
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

    std::vector<Treeinfo> Treeplace[TREENUMBER];

    Noise* generator;
    Texture2D HeightMap, CloudMap;
    Texture2D NormalMap0, NormalMap1, pNormalMap;

    Water* water;
    Plants* plant[TREENUMBER];

    Shader map_shader, map_instance_shader, water_shader, shadow_shader;
    GLuint VAO, VBO, instanceVAO, instanceVBO;
    std::vector<GLfloat> vertices;

    Scene(glm::vec3 initpos, Shader shader);
    ~Scene();
    void Initialize();
    void InitBuffer();
    void draw(const glm::mat4& view, const glm::mat4& PVMatrix, const glm::mat4& lightSpaceMatrix, const Texture2D& ShadowMap, const Texture2D& BluredShadow);
    void drawssr(Shader shader, const glm::mat4& PVMatrix, const glm::mat4& viewmat, const glm::mat4& lightSpaceMatrix, const Texture2D& BluredShadow);
    void generate_scene();
    void generate_cloud(GLfloat dt);

    void UpdateChunks(glm::vec3 Position);
    void UpdateNeighbor(GLint x, GLint y);
    void GetLocationbyCamera(glm::vec3 Position, GLint& cx, GLint& cz, GLint& ms, GLint& mz);
    void UpdateTreeplace();
    bool ValidPlace(int cx, int cz, int mx, int mz);
    glm::vec3 FindStartFinishLocation(int type);

    Texture2D Generate_HeightMap();
    Texture2D Generate_CloudMap();
    Texture2D Generate_NormalMap(int th);
    Texture2D Generate_pNormalMap();
    void Generate_Treeplace();
    void Generate_Rockplace();
    int PlaceEnable(int i, int j, int k, int h, bool insea);
    void Generate_ShadowMap(const glm::mat4& lightSpaceMatrix, const glm::mat4& view);
private:

};


#endif //TMPER_SCENE_H
