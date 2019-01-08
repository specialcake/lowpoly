//
// Created by dydxh on 1/8/19.
//

#include "Bloom.h"
#include "gaussblur.h"

Shader Bloom::bloomShader;
Gaussblur Bloom::Blurer;

void Bloom::Initialize(const Shader &shader) {
    bloomShader = shader;
}

void Bloom::RenderBloom(PostProcessor* SceneMap) {
    GLuint BluredID = Blurer.GaussBlur(SceneMap->BrightTexture, 20);

    bloomShader.use();
    glActiveTexture(GL_TEXTURE0);
    SceneMap->ColorTexture.Bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, BluredID);

    glBindVertexArray(SceneMap->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
