//
// Created by dydxh on 18-7-17.
//
#include "mesh.h"
#include "shader.h"
#include "../utils/tools.h"
#include "../config.h"
#include <string>
#include <vector>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec4 colors) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->color = colors;

//    setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
glm::vec3 Mesh::GetCenter() {
    int amount = vertices.size();
    glm::vec3 ret = glm::vec3(0.0f);
    for(int i = 0; i < amount; i++)
        ret += vertices[i].Position;
    ret /= amount;
    return ret;
}
void Mesh::Draw(Shader shader) {
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    if(textures.size() > 0) {
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string name = textures[i].type;
            if (name == MODEL_DIFFUSE_NAME) {
                name += std::to_string(diffuseNumber++);
            } else if (name == MODEL_SPECULAR_NAME) {
                name += std::to_string(specularNumber++);
            }
            shader.setInt(name, i);
            shader.setInt(name + "_exist", 1);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }
    else{
        shader.setInt("texture_diffuse1_exist", 0);
        shader.setVec4("texture_diffuse1_color", this->color);
//        Tools::PrintVec4(this->color);
    }
    glBindVertexArray(VAO);
//    std::cout << indices.size() << std::endl;
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}