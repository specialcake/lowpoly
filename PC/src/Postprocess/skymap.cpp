//
// Created by dydxh on 12/22/18.
//

#include "skymap.h"
#include "../config.h"

Skymap::Skymap(Shader shader) : shader(shader) {
    Initialize();
    InitRenderData();
}

Skymap::~Skymap() {}

void Skymap::Initialize() {
    glGenFramebuffers(1, &this->SkymapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->SkymapFBO);
    for(int i = 0; i < 6; i++){
        skymap[i].GenerateSkymap(SCREEN_WIDTH, SCREEN_HEIGHT);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + i,
            GL_TEXTURE_2D,
            skymap[i].ID, 0
        );
    }

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[6] = {GL_COLOR_ATTACHMENT0,
                                   GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2,
                                   GL_COLOR_ATTACHMENT3,
                                   GL_COLOR_ATTACHMENT4,
                                   GL_COLOR_ATTACHMENT5};
    glDrawBuffers(6, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "[skymap]: Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Skymap::BeginMakeMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, SkymapFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
}

void Skymap::Draw(glm::vec3 SunPos) {
    shader.setVec3("uSunPos", SunPos);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Skymap::EndMakeMap() {
    glEnable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Skymap::InitRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}