//
// Created by dydxh on 18-7-17.
//

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <string>
#include <vector>

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture{
    unsigned int id;
    std::string path;
    std::string type;
};

class Mesh{
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    glm::vec4 color;
    Mesh(){}
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec4 colors);
    void Draw(Shader shader);
    void setupMesh();
private:
    unsigned int VBO, EBO;
};


#endif //CHAPTER3_MESH_H
