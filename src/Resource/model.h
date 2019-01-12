//
// Created by dydxh on 18-7-17.
//

#ifndef MODEL_H
#define MODEL_H
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include "shader.h"
#include "mesh.h"
#include <string>
#include <vector>
#include <map>

struct Trunk{
    glm::vec3 bottom;
    GLfloat height, Radius;
};

unsigned int TextureFromFile(const char *path, const std::string &directory);

class Model{
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::map<std::string, Mesh*> group;
    std::string directory;
    GLfloat minx, maxx, miny, maxy, minz, maxz;
    GLfloat cx, cy, cz, Gx, Gy, Gz, total_count;
    GLfloat delta_x, delta_y, delta_z;

    Model(){};
    Model(const char *path, bool center);
    void SetBias(GLfloat dx, GLfloat dy, GLfloat dz);
    glm::vec3 BiasVector();
    Trunk GetTrunk(std::string name);
    void Draw(Shader shader);
private:
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
