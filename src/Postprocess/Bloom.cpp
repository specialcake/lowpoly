//
// Created by dydxh on 1/8/19.
//

#include "Bloom.h"
#include "gaussblur.h"

GLuint Bloom::FBO;
GLuint Bloom::VAO;
GLuint Bloom::ColorBuffer[2];
Shader Bloom::bloomShader;

void Bloom::Initialize(const Shader &shader) {
    bloomShader = shader;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glGenTextures(2, ColorBuffer);
    for(int i = 0; i < 2; i++){
        glBindTexture(GL_TEXTURE_2D, ColorBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorBuffer[i], 0);
    }
    GLuint attachments[2] = {GL_COLOR_ATTACHMENT0,
                             GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
    glReadBuffer(GL_NONE);
    initRenderData();
}

void Bloom::RenderBloom() {
    ColorBuffer[1] = Gaussblur::GaussBlur(ColorBuffer[1]);
    bloomShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ColorBuffer[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ColorBuffer[1]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Bloom::BeginMakeMap(){
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //glCullFace(GL_BACK);
}

void Bloom::EndMakeMap() {
//    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::initRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
            // Pos        // Tex
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GL_FLOAT)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
