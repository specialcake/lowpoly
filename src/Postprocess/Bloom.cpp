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
    SceneMap->BrightTexture.ID = Blurer.GaussBlur(SceneMap->BrightTexture);

    bloomShader.use();
    glActiveTexture(GL_TEXTURE0);
    SceneMap->ColorTexture.Bind();
    glActiveTexture(GL_TEXTURE1);
    SceneMap->BrightTexture.Bind();

    glBindVertexArray(SceneMap->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
