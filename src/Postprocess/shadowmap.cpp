//
// Created by dydxh on 12/4/18.
//

#include "shadowmap.h"
#include "../config.h"

Shadowmap::Shadowmap() {
    this->Initialize();
}
Shadowmap::~Shadowmap() {
    glDeleteFramebuffers(1, &DepthMapFBO);
}
void Shadowmap::Initialize() {
    glGenFramebuffers(1, &DepthMapFBO);
    DepthMap.GenerateShadowMap(SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap.ID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 Shadowmap::BeginMakeMap(){
    GLfloat near_plane = 0.1f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
    glm::mat4 lightview = glm::lookAt(-PARLIGHT_DIR, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightview;
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    return lightSpaceMatrix;
}

void Shadowmap::EndMakeMap() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}