//
// Created by dydxh on 11/19/18.
//

#include "submesh.h"
#include "../config.h"
#include "chunk.h"
#include "scene.h"
#include "../utils/tools.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <iostream>

Submesh::Submesh(GLint x, GLint z) : pos_x(x), pos_z(z) {
    this->Initialize();
}
Submesh::~Submesh() {

}

void Submesh::Initialize() {
    this->xPos = this->xNeg = this->zPos = this->zNeg = nullptr;
    this->Occupied = new GLboolean[OBJECT_TYPE_NUMBER];
    this->MeshType = LAND;
    memset(this->Occupied, GL_FALSE, sizeof(GLboolean) * OBJECT_TYPE_NUMBER);
}

void Submesh::recycle(GLint x, GLint z) {
    this->xPos = this->xNeg = this->zPos = this->zNeg = nullptr;
    this->pos_x = x, this->pos_z = z;
    memset(this->Occupied, GL_FALSE, sizeof(GLboolean) * OBJECT_TYPE_NUMBER);
}

void Submesh::draw_map() {
//    static bool debugflag = true;
    static Scene* top_scene = this->parent->parent;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->get_Position());
    model = glm::scale(model, glm::vec3(MESH_LENGTH));
    top_scene->map_shader.setMat4("model", model);

    glBindBuffer(GL_ARRAY_BUFFER, top_scene->VBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(GLint k = 0; k < 6; k++){
        auto delta_x = (GLint)top_scene->vertices[k * 3 + 0], delta_z = (GLint)top_scene->vertices[k * 3 + 2];
        top_scene->vertices[k * 3 + 1] = (this->parent->height[pos_x + delta_x][pos_z + delta_z] * 0.5f + 0.5f) * MAX_HEIGHT;
//        std::cout<<this->parent->height[pos_x + delta_x][pos_z + delta_z]<<std::endl;
    }
    memcpy(ptr, &top_scene->vertices[0], sizeof(GLfloat) * top_scene->vertices.size());
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
//    debugflag = false;
}

void Submesh::draw_water() {
    static Scene* top_scene = this->parent->parent;
    top_scene->water_shader.setVec3("meshoffset", this->get_Position());
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

glm::vec3 Submesh::get_Position() {
    static Scene* top_scene = this->parent->parent;
    glm::vec3 ret = top_scene->offset;
    ret += glm::vec3((this->parent->pos_x - CHUNK_RADIUS) * CHUNK_LENGTH,
                     0.0f,
                     (this->parent->pos_z - CHUNK_RADIUS) * CHUNK_LENGTH);
    ret += glm::vec3((this->pos_x - MESH_RADIUS - 0.5f) * MESH_LENGTH,
                     0.0f,
                     (this->pos_z - MESH_RADIUS - 0.5f) * MESH_LENGTH);
    return ret;
    return top_scene->offset
           + top_scene->chunk_offset[this->parent->pos_x][this->parent->pos_z]
           + top_scene->mesh_offset[this->pos_x][this->pos_z];
}