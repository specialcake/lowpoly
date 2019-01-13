//
// Created by dydxh on 18-7-17.
//
#include "model.h"
#include "shader.h"
#include "mesh.h"
#include "../config.h"
#include <stb_image.h>
#include <iostream>
#include <cmath>
#include <map>

Model::Model(const char *path, bool center) {
    maxx = maxy = maxz = -10000000.0f;
    minx = miny = minz = 10000000.0f;
    Gx = Gy = Gz = 0.0f;
    delta_x = delta_y = delta_z = 0.0f;
    total_count = 0.0f;
    loadModel(path);
    cx = (minx + maxx) / 2.0f;
    cy = (miny + maxy) / 2.0f;
    cz = (minz + maxz) / 2.0f;
    Gx /= total_count, Gy /= total_count, Gz /= total_count;

    glm::vec3 delta = glm::vec3(-Gx, -miny, -Gz);

    for(int i = 0; i < meshes.size(); i++){
        if(center) {
            for (int j = 0; j < meshes[i].vertices.size(); j++)
                meshes[i].vertices[j].Position += delta;
        }
        meshes[i].setupMesh();
    }

//    printf("Gx: %f, Gy: %f, Gz: %f\n", Gx, Gy, Gz);
//    printf("cx: %f, cy: %f, cz: %f\n", cx, cy, cz);
//    printf("min x: %f, y: %f, z: %f\n", minx ,miny, minz);
//    printf("max x: %f, y: %f, z: %f\n", maxx, maxy, maxz);
}
void Model::Draw(Shader shader) {
    for(unsigned int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(shader);
    }
}
void Model::SetBias(GLfloat dx, GLfloat dy, GLfloat dz) {
    delta_x = dx, delta_y = dy, delta_z = dz;
}
glm::vec3 Model::BiasVector() {
    return glm::vec3(delta_x, delta_y, delta_z);
}
glm::vec3 Model::GetCenter(std::string name) {
    Mesh* meshptr = group[name];
    glm::vec3 center = glm::vec3(0.0f);
    for(int i = 0; i < meshptr->vertices.size(); i++){
        center += meshptr->vertices[i].Position;
    }
    center /= glm::vec3(meshptr->vertices.size());
    return center;
}
Trunk Model::GetTrunk(std::string name) {
    Mesh* trunk = group[name];
    Trunk ret = (Trunk){glm::vec3(0.0f), 0.0f, 0.0f};
    glm::vec3 center = glm::vec3(0.0f);
    GLfloat miny = 20000.0f, maxy = -20000.0f;
    for(int i = 0; i < trunk->vertices.size(); i++){
        center += trunk->vertices[i].Position;
        miny = glm::min(miny, trunk->vertices[i].Position.y);
        maxy = glm::max(miny, trunk->vertices[i].Position.y);
    }
    center /= trunk->vertices.size();
    ret.bottom = center; ret.bottom.y = miny;
    ret.height = maxy - miny;
    for(int i = 0; i < trunk->vertices.size(); i++){
        glm::vec3 delta = trunk->vertices[i].Position - center;
        GLfloat dist = glm::length(glm::vec2(delta.x, delta.z));
        ret.Radius += dist;
    }
    ret.Radius /= trunk->vertices.size();
    return ret;
}
BoundBox Model::GetBoundBox(){
    BoundBox ret = (BoundBox){glm::vec3(0.0f), 0.0f, 0.0f, 0.0f};
    ret.Front = maxz - minz;
    ret.Left  = maxx - minx;
    ret.Down  = maxy - miny;
    ret.Point = glm::vec3(maxx - Gx, maxy - Gy, maxz - Gz);
    return ret;
}
void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
//    std::cout << "textureloaded : " << this->textures_loaded.size() << std::endl;
//    std::cout << "meshesloaded : " << this->meshes.size() << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        group[mesh->mName.C_Str()] = &meshes[meshes.size() - 1];
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex v;
        v.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]){
            v.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else{
            v.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(v);

        minx = glm::min(minx, v.Position.x); miny = glm::min(miny, v.Position.y); minz = glm::min(minz, v.Position.z);
        maxx = glm::max(maxx, v.Position.x); maxy = glm::max(maxy, v.Position.y); maxz = glm::max(maxz, v.Position.z);
        this->Gx += v.Position.x;this->Gy += v.Position.y;this->Gz += v.Position.z;
        this->total_count += 1.0f;
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, MODEL_DIFFUSE_NAME);
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, MODEL_SPECULAR_NAME);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
    aiColor4D diffuse (0.0f, 0.0f, 0.0f, 1.0f);
    int success = aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
    if(success == AI_SUCCESS){
        color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    }
    else{
        std::cout << "[Error] Get mesh color" << std::endl;
    }
    return Mesh(vertices, indices, textures, color);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



