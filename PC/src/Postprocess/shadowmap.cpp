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

void Shadowmap::UpdateFrustum(const Scene *scene) {
    glm::mat4 lightviewmat = glm::lookAt(PARLIGHT_POSITION + scene->offset,
                                         PARLIGHT_POSITION + scene->offset + PARLIGHT_DIR,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
    this->near = this->left  = this->bottom = 2000000.0f;
    this->far  = this->right = this->top    = -2000000.0f;
    for(int i = 0; i < CHUNK_SIZE; i++){
        for(int j = 0; j < CHUNK_SIZE; j++){
            for(int k = 0; k <= MESH_SIZE; k++){
                for(int h = 0; h <= MESH_SIZE; h++){
                    glm::vec3 pos = scene->offset +
                    scene->chunk_offset[i][j] +
                    scene->mesh_offset[k][h];
                    pos.y = scene->chunk[i][j]->height[k][h];
                    pos = glm::vec3(lightviewmat * glm::vec4(pos, 1.0f));
                    this->left   = glm::min(this->left,   pos.x);
                    this->right  = glm::max(this->right,  pos.x);
                    this->bottom = glm::min(this->bottom, pos.y);
                    this->top    = glm::max(this->top,    pos.y);
                    this->near   = glm::min(this->near,  -pos.z);
                    this->far    = glm::max(this->far ,  -pos.z);
                }
            }
        }
    }
    this->left -= 0.2f, this->bottom -= 0.2f, this->near -= 0.2f;
    this->right += 0.2f, this->top += 0.2f, this->far += 0.2f;
}

glm::mat4 Shadowmap::GetlightSpaceMatrix(const Scene* scene) {
    glm::mat4 lightProjection = glm::ortho(this->left, this->right,
               this->bottom, this->top,
               this->near, this->far);
    glm::mat4 lightviewmat = glm::lookAt(PARLIGHT_POSITION + scene->offset,
                                         PARLIGHT_POSITION + scene->offset + PARLIGHT_DIR,
                                         glm::vec3(0.0f, 1.0f, 0.0f));
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