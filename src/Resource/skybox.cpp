//
// Created by dydxh on 12/18/18.
//

#include <stb_image.h>
#include "skybox.h"
#include "../config.h"

Skybox::Skybox(Shader shader) : shader(shader) {
    Initialize();
}

Skybox::~Skybox() {

}

void Skybox::Initialize() {
    GLfloat skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f, 4.0f,
        -1.0f, -1.0f, -1.0f, 4.0f,
        1.0f, -1.0f, -1.0f, 4.0f,
        1.0f, -1.0f, -1.0f, 4.0f,
        1.0f,  1.0f, -1.0f, 4.0f,
        -1.0f,  1.0f, -1.0f, 4.0f,

        -1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f,

        1.0f, -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 0.0f,

        -1.0f, -1.0f,  1.0f, 5.0f,
        -1.0f,  1.0f,  1.0f, 5.0f,
        1.0f,  1.0f,  1.0f, 5.0f,
        1.0f,  1.0f,  1.0f, 5.0f,
        1.0f, -1.0f,  1.0f, 5.0f,
        -1.0f, -1.0f,  1.0f, 5.0f,

        -1.0f,  1.0f, -1.0f, 2.0f,
        1.0f,  1.0f, -1.0f, 2.0f,
        1.0f,  1.0f,  1.0f, 2.0f,
        1.0f,  1.0f,  1.0f, 2.0f,
        -1.0f,  1.0f,  1.0f, 2.0f,
        -1.0f,  1.0f, -1.0f, 2.0f,

        -1.0f, -1.0f, -1.0f, 3.0f,
        -1.0f, -1.0f,  1.0f, 3.0f,
        1.0f, -1.0f, -1.0f, 3.0f,
        1.0f, -1.0f, -1.0f, 3.0f,
        -1.0f, -1.0f,  1.0f, 3.0f,
        1.0f, -1.0f,  1.0f, 3.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

//void Skybox::LoadTexture(Texture2D *skymaps) {
//    GLfloat* data[6];
//    for(int i = 0; i < 6; i++){
//        data[i] = new GLfloat[4 * SCREEN_WIDTH * SCREEN_HEIGHT];
//        skymaps[i].Bind();
//        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data[i]);
//    }
//    glGenTextures(1, &CubeMapid);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapid);
//
//    glActiveTexture(GL_TEXTURE0);
//    for(unsigned int i = 0; i < 6; i++) {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                     0, GL_RGBA,
//                     SCREEN_WIDTH, SCREEN_HEIGHT,
//                     0, GL_RGBA, GL_FLOAT, data[i]);
//    }
//    for(unsigned int i = 0; i < 6; i++){
//        delete[] data[i];
//    }
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//}

void Skybox::Draw(Texture2D* skymaps) {
    glBindVertexArray(VAO);
    for(int i = 0; i < 6; i++){
        glActiveTexture(GL_TEXTURE0 + i);
        skymaps[i].Bind();
    }
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}