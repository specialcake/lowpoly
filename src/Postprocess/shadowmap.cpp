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

glm::mat4 Shadowmap::GetlightSpaceMatrix() {
    glm::mat4 lightProjection = glm::ortho(-40.0f, 45.0f, -12.0f, 14.0f, 0.1f, 150.0f);
    glm::mat4 lightviewmat = glm::lookAt(PARLIGHT_POSITION, PARLIGHT_POSITION + PARLIGHT_DIR, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightviewmat;
    return lightSpaceMatrix;
}

void Shadowmap::BeginMakeMap(){
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
}

void Shadowmap::EndMakeMap() {
//    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}