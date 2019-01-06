//
// Created by dydxh on 1/6/19.
//

#include "SSR.h"

SSR::SSR(Shader scene_shader, Shader model_shader, Shader ssr_shader) {
    this->SceneShader = scene_shader;
    this->ModelShader = model_shader;
    this->SSRShader = ssr_shader;
    this->Initialize();
    this->InitRenderData();
}
SSR::~SSR() {
    glDeleteFramebuffers(1, &SSRFBO);
}

void SSR::Initialize(){
    DepthMap.GenerateShadowMap(SCREEN_WIDTH, SCREEN_HEIGHT);
    NormalMap.GenerateSkymap(SCREEN_WIDTH, SCREEN_HEIGHT);
    ColorMap.GenerateSkymap(SCREEN_WIDTH, SCREEN_HEIGHT);
    Reflectable.GenerateSkymap(SCREEN_WIDTH, SCREEN_HEIGHT);

    glGenFramebuffers(1, &SSRFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, SSRFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap.ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorMap.ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalMap.ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, Reflectable.ID, 0);
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0,
                                   GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "[SSR]: SSR FrameBuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSR::BeginMakeUp() {
    glBindFramebuffer(GL_FRAMEBUFFER, SSRFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
}

void SSR::EndMakeUp() {
    glEnable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SSR::Render() {
    this->SSRShader.use();
    glActiveTexture(GL_TEXTURE0);
    DepthMap.Bind();
    glActiveTexture(GL_TEXTURE1);
    NormalMap.Bind();
    glActiveTexture(GL_TEXTURE2);
    ColorMap.Bind();
    glActiveTexture(GL_TEXTURE3);
    Reflectable.Bind();
    SSRShader.setFloat("near", NEAR_PLANE);
    SSRShader.setFloat("far", FAR_PLANE);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void SSR::InitRenderData(){
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos
         1.0f,  1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,

         1.0f, -1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



