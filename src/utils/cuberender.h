#ifndef TMPER_CUBERENDER_H
#define TMPER_CUBERENDER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Resource/texture.h"
#include "../Resource/shader.h"
#include "../Resource/resourcemanager.h"

class CubeRender {
public:
    Shader shader;
    GLuint cubeVAO;
    CubeRender(Shader shader);
    ~CubeRender();

    void DrawCube(glm::mat4 PVMatrix, glm::vec3 position, glm::vec3 size = glm::vec3(1.0, 1.0, 1.0));
    void initRenderData();
};


#endif //TMPER_CUBERENDER_H
