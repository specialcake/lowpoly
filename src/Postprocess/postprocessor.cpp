//
// Created by dydxh on 10/24/18.
//

#include "postprocessor.h"

#include <iostream>

PostProcessor::PostProcessor() {
    Initialize();
}

void PostProcessor::Initialize() {
    Width = SCREEN_WIDTH, Height = SCREEN_HEIGHT;
    DepthTexture.GenerateShadowMap(Width, Height);
    ColorTexture.GenerateSkymap(Width, Height);
    BrightTexture.GenerateSkymap(Width, Height);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture.ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTexture.ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, BrightTexture.ID, 0);

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0,
                                   GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "[PostProcess]: FrameBuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    initRenderData();
}

void PostProcessor::BeginRender() {
    glViewport(0, 0, Width, Height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void PostProcessor::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Width, Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessor::initRenderData() {
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
