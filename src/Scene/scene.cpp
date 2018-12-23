//
// Created by dydxh on 11/19/18.
//

#include "scene.h"
#include "../Resource/resourcemanager.h"

Scene::Scene(glm::vec3 initpos, Shader shader) : offset(initpos), map_shader(shader) {
    this->Initialize();
}

Scene::~Scene() {

}

void Scene::Initialize() {
    for(GLint i = 0; i < 3; i++){
        this->tmp_chunk[i] = new Chunk(0, 0);
        this->tmp_chunk[i]->parent = this;
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        for(GLint j = 0; j < CHUNK_SIZE; j++) {
            this->chunk[i][j] = new Chunk(i, j);
            this->chunk[i][j]->parent = this;
        }
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        for(GLint j = 0; j < CHUNK_SIZE; j++) {
            this->UpdateNeighbor(i, j);
        }
    }

    this->cur_Chunk = this->chunk[CHUNK_RADIUS][CHUNK_RADIUS];
    this->cur_Submesh = this->cur_Chunk->submesh[MESH_RADIUS][MESH_RADIUS];
    this->InitBuffer();

    this->generator = new Noise(NOISE_SIZE);
    this->water = new Water();

    for(GLint i = 0; i < CHUNK_SIZE; i++){
        for(GLint j = 0; j < CHUNK_SIZE; j++){
            chunk_offset[i][j] = glm::vec3((i - CHUNK_RADIUS) * CHUNK_LENGTH, 0.0f, (j - CHUNK_RADIUS) * CHUNK_LENGTH);
        }
    }
    for(GLint i = 0; i <= MESH_SIZE; i++){
        for(GLint j = 0; j <= MESH_SIZE; j++){
            mesh_offset[i][j] = glm::vec3((i - MESH_RADIUS - 0.5f) * MESH_LENGTH, 0.0f, (j - MESH_RADIUS - 0.5f) * MESH_LENGTH);
        }
    }
}
void Scene::InitBuffer() {
    this->vertices = {
            // Pos
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLfloat instance_vertices[] = {
            1.0f, 1.0f, -1.0f,
            1.0f, 0.0f, -1.0f,
            0.0f, 1.0f, -1.0f,

            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &this->instanceVAO);
    glGenBuffers(1, &this->instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instance_vertices), &instance_vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->instanceVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Scene::generate_scene() {
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        for (GLint j = 0; j < CHUNK_SIZE; j++) {
            chunk[i][j]->generate_height();
        }
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        for (GLint j = 0; j < CHUNK_SIZE; j++) {
            chunk[i][j]->Updateinfo();
        }
    }
    std::cout << this->chunk[0][0]->height[0][0] << std::endl;
    glm::vec3 va = glm::vec3(0.0f, 1.10733f, 1.0404f);
    glm::vec3 vb = glm::vec3(1.0404f, -0.552877f, 0.0f);
    glm::vec3 vc = glm::cross(va, vb);
    Tools::PrintVec3(glm::normalize(vc));
//    this->chunk[0][0]->height[1][1] = 3.0f;
}
void Scene::draw(const glm::mat4& PVMatrix, const glm::mat4& lightSpaceMatrix,
                 const Texture2D& ShadowMap, const Texture2D& BluredShadow) {
    if(ResourceManager::Keys[GLFW_KEY_H]){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
    map_instance_shader.use();
    glActiveTexture(GL_TEXTURE0);
    this->HeightMap.Bind();
    glActiveTexture(GL_TEXTURE4);
    ShadowMap.Bind();
    glActiveTexture(GL_TEXTURE5);
    BluredShadow.Bind();
    map_instance_shader.setMat4("PVMatrix", PVMatrix);
    map_instance_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    map_instance_shader.setVec3("lower_color", LOWER_COLOR);
    map_instance_shader.setVec3("land_color", LAND_COLOR);
    map_instance_shader.setVec3("rock_color", ROCK_COLOR);
    map_instance_shader.setFloat("scalefactor", MESH_LENGTH);
    map_instance_shader.setVec3("scene_offset", this->offset);
    map_instance_shader.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);
    map_instance_shader.setFloat("near_plane", NEAR_PLANE);
    map_instance_shader.setFloat("far_plane", FAR_PLANE);
    map_instance_shader.setLight(ResourceManager::camera.Position);

    glBindVertexArray(this->instanceVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);
    if(ResourceManager::Keys[GLFW_KEY_H]){
        glCullFace(GL_BACK);
    }

#ifdef viewnormal
    Shader normvis = ResourceManager::GetShader("normvis");
    normvis.use();
    glActiveTexture(GL_TEXTURE0);
    this->HeightMap.Bind();
    normvis.setMat4("PVMatrix", PVMatrix);
    normvis.setMat4("projection", glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f));
    normvis.setMat4("view", ResourceManager::camera.GetViewMatrix());
    normvis.setFloat("scalefactor", MESH_LENGTH);
    normvis.setVec3("scene_offset", this->offset);
    normvis.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);
    normvis.setVec3("lightdir", PARLIGHT_DIR);
    glBindVertexArray(this->instanceVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);
#endif //viewnormal

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water_shader.use();
    glActiveTexture(GL_TEXTURE0);
    ShadowMap.Bind();
    glActiveTexture(GL_TEXTURE1);
    BluredShadow.Bind();
    water_shader.setMat4("PVMatrix", PVMatrix);
    water_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    water_shader.setFloat("water_height", SEA_LEVEL);
    water_shader.setFloat("scalefactor", MESH_LENGTH);
    water_shader.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);
    water_shader.setVec3("scene_offset", this->offset);
    water_shader.setVec3("water_color", SEA_COLOR);
    water_shader.setLight(ResourceManager::camera.Position);
    this->water->Setwater(water_shader);

    glBindVertexArray(this->VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_BLEND);
}

void Scene::Generate_ShadowMap(const glm::mat4& lightSpaceMatrix, const glm::mat4& view) {
    shadow_shader.use();
    glActiveTexture(GL_TEXTURE0);
    this->HeightMap.Bind();
    shadow_shader.setMat4("PVMatrix", lightSpaceMatrix);
    shadow_shader.setMat4("view", view);
    shadow_shader.setFloat("scalefactor", MESH_LENGTH);
    shadow_shader.setVec3("scene_offset", this->offset);
    shadow_shader.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);
    shadow_shader.setVec3("lightdir", PARLIGHT_DIR);
    shadow_shader.setFloat("near_plane", NEAR_PLANE);
    shadow_shader.setFloat("far_plane", FAR_PLANE);

    glBindVertexArray(this->instanceVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);
}

void Scene::UpdateChunks() {
    this->debugflag = false;
    Direction dir = ORIGIN_POS;
    GLint Chunkx, Chunkz, Meshx, Meshz;
    GetLocationbyCamera(Chunkx, Chunkz, Meshx, Meshz);
//    printf("(%d, %d) -> (%d, %d)\n", Chunkx, Chunkz, Meshx, Meshz);
    if(Chunkx == cur_Chunk->pos_x && Chunkz == cur_Chunk->pos_z){
        cur_Submesh = cur_Chunk->submesh[Meshx][Meshz];
        ResourceManager::dir = ORIGIN_POS;
        return ;
    }
    this->debugflag = true;
    static auto toUpdate = new Chunk*[CHUNK_SIZE];
    static GLint head, tail;
    head = tail = 0;
    if(Chunkx > cur_Chunk->pos_x){
        this->offset += glm::vec3(CHUNK_LENGTH, 0.0f, 0.0f);
        for(GLint i = 0; i < CHUNK_SIZE; i++) {
            Chunk* tmpChunk = this->chunk[0][i];
            for (GLint j = 1; j < CHUNK_SIZE; j++) {
                this->chunk[j - 1][i] = this->chunk[j][i];
                this->chunk[j - 1][i]->pos_x = j - 1;
            }
            this->chunk[CHUNK_SIZE - 1][i] = tmpChunk;
            tmpChunk->recycle(CHUNK_SIZE - 1, i);
            toUpdate[tail++] = tmpChunk;
        }
        dir = MOVE_XPOS;
    }
    else if(Chunkx < cur_Chunk->pos_x){
        this->offset -= glm::vec3(CHUNK_LENGTH, 0.0f, 0.0f);
        for(GLint i = 0; i < CHUNK_SIZE; i++) {
            Chunk* tmpChunk = this->chunk[CHUNK_SIZE - 1][i];
            for (GLint j = CHUNK_SIZE - 1; j > 0; j--){
                this->chunk[j][i] = this->chunk[j - 1][i];
                this->chunk[j][i]->pos_x = j;
            }
            this->chunk[0][i] = tmpChunk;
            tmpChunk->recycle(0, i);
            toUpdate[tail++] = tmpChunk;
        }
        dir = MOVE_XNEG;
    }
    if(Chunkz > cur_Chunk->pos_z){
        this->offset += glm::vec3(0.0f, 0.0f, CHUNK_LENGTH);
        for(GLint i = 0; i < CHUNK_SIZE; i++) {
            Chunk* tmpChunk = this->chunk[i][0];
            for (GLint j = 1; j < CHUNK_SIZE; j++){
                this->chunk[i][j - 1] = this->chunk[i][j];
                this->chunk[i][j - 1]->pos_z = j - 1;
            }
            this->chunk[i][CHUNK_SIZE - 1] = tmpChunk;
            tmpChunk->recycle(i, CHUNK_SIZE - 1);
            toUpdate[tail++] = tmpChunk;
        }
        dir = MOVE_ZPOS;
    }
    else if(Chunkz < cur_Chunk->pos_z){
        this->offset -= glm::vec3(0.0f, 0.0f, CHUNK_LENGTH);
        for(GLint i = 0; i < CHUNK_SIZE; i++) {
            Chunk* tmpChunk = this->chunk[i][CHUNK_SIZE - 1];
            for (GLint j = CHUNK_SIZE - 1; j > 0; j--){
                this->chunk[i][j] = this->chunk[i][j - 1];
                this->chunk[i][j]->pos_z = j;
            }
            this->chunk[i][0] = tmpChunk;
            tmpChunk->recycle(i, 0);
            toUpdate[tail++] = tmpChunk;
        }
        dir = MOVE_ZNEG;
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        this->UpdateNeighbor(0, i);
        this->UpdateNeighbor(1, i);
        this->UpdateNeighbor(i, 0);
        this->UpdateNeighbor(i, 1);
        this->UpdateNeighbor(CHUNK_SIZE - 1, i);
        this->UpdateNeighbor(CHUNK_SIZE - 2, i);
        this->UpdateNeighbor(i, CHUNK_SIZE - 1);
        this->UpdateNeighbor(i, CHUNK_SIZE - 2);
    }
    while(head < tail){
        toUpdate[head++]->Updateinfo();
    }
    cur_Chunk = this->chunk[CHUNK_RADIUS][CHUNK_RADIUS];
    cur_Submesh = cur_Chunk->submesh[MESH_RADIUS][MESH_RADIUS];

    ResourceManager::dir = dir;
}
void Scene::UpdateNeighbor(GLint x, GLint y) {
    if(x > 0) chunk[x][y]->xNeg = chunk[x - 1][y];
    else chunk[x][y]->xNeg = nullptr;
    if(y > 0) chunk[x][y]->zNeg = chunk[x][y - 1];
    else chunk[x][y]->zNeg = nullptr;
    if(x < CHUNK_SIZE - 1) chunk[x][y]->xPos = chunk[x + 1][y];
    else chunk[x][y]->xPos = nullptr;
    if(y < CHUNK_SIZE - 1) chunk[x][y]->zPos = chunk[x][y + 1];
    else chunk[x][y]->zPos = nullptr;
}

void Scene::GetLocationbyCamera(GLint &cx, GLint &cz, GLint &mx, GLint &mz) {
    glm::vec3 position = ResourceManager::camera.Position -
                         chunk[CHUNK_RADIUS][CHUNK_RADIUS]->submesh[MESH_RADIUS][MESH_RADIUS]->get_Position() -
                         glm::vec3(MESH_LENGTH / 2.0f, 0.0f, MESH_LENGTH / 2.0f);
    position = glm::vec3(0.0f) -
               chunk[CHUNK_RADIUS][CHUNK_RADIUS]->submesh[MESH_RADIUS][MESH_RADIUS]->get_Position() -
               glm::vec3(MESH_LENGTH / 2.0f, 0.0f, MESH_LENGTH / 2.0f);
//    printf("camera position = ");
//    Tools::PrintVec3(ResourceManager::camera.Position);
//    printf("center position = ");
//    Tools::PrintVec3(chunk[CHUNK_RADIUS][CHUNK_RADIUS]->submesh[MESH_RADIUS][MESH_RADIUS]->get_Position() + glm::vec3(MESH_LENGTH / 2.0f, 0.0f, MESH_LENGTH / 2.0f));

    if(position.x > 0)
        cx = static_cast<GLint>(position.x / CHUNK_LENGTH + 0.5f) + CHUNK_RADIUS;
    else
        cx = static_cast<GLint>(position.x / CHUNK_LENGTH - 0.5f) + CHUNK_RADIUS;
    if(position.z > 0)
        cz = static_cast<GLint>(position.z / CHUNK_LENGTH + 0.5f) + CHUNK_RADIUS;
    else
        cz = static_cast<GLint>(position.z / CHUNK_LENGTH - 0.5f) + CHUNK_RADIUS;
    position.x -= (cx - CHUNK_RADIUS) * CHUNK_LENGTH;
    position.z -= (cz - CHUNK_RADIUS) * CHUNK_LENGTH;
    if(position.x > 0)
        mx = static_cast<GLint>(position.x / MESH_LENGTH + 0.5f) + MESH_RADIUS;
    else
        mx = static_cast<GLint>(position.x / MESH_LENGTH - 0.5f) + MESH_RADIUS;
    if(position.z > 0)
        mz = static_cast<GLint>(position.z / MESH_LENGTH + 0.5f) + MESH_RADIUS;
    else
        mz = static_cast<GLint>(position.z / MESH_LENGTH - 0.5f) + MESH_RADIUS;
}

Texture2D Scene::Generate_HeightMap() {
    GLint p = 0;
    GLint limit = CHUNK_SIZE * CHUNK_SIZE * MESH_SIZE * MESH_SIZE + CHUNK_SIZE * MESH_SIZE * 2 + 1;
    GLuint len = CHUNK_SIZE * MESH_SIZE + 1;
    static GLfloat* data = new GLfloat[limit];
    for(GLint i = 0; i < CHUNK_SIZE; i++){
        for(GLint j = 0; j < MESH_SIZE; j++){
            for(GLint k = 0; k < CHUNK_SIZE; k++){
                if(this->debugflag) {
                    for (GLint h = 0; h < MESH_SIZE; h++) {
                        printf("%.3lf ", chunk[i][k]->height[j][h]);
                    }
                    if(k == CHUNK_SIZE - 1){
                        printf("%.3lf", chunk[i][k]->height[j][MESH_SIZE]);
                    }
                }
                memcpy(data + p, chunk[i][k]->height[j], sizeof(GLfloat) * (MESH_SIZE + 1));
                p += MESH_SIZE;
            }
            p++;
            if(this->debugflag) {
                printf("\n");
            }
        }
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++){
        memcpy(data + p, chunk[CHUNK_SIZE - 1][i]->height[MESH_SIZE], sizeof(GLfloat) * (MESH_SIZE + 1));
        p += MESH_SIZE;
    }
    p++;
    for(GLint i = 0; i < limit; i++){
        data[i] = data[i] * 0.1f;
    }
    return ResourceManager::MakeTexture(len, len, GL_RED, data, "HeightMap");
}
Texture2D Scene::Generate_NormalMap(int th) {
    GLint p = 0;
    GLint limit = CHUNK_SIZE * CHUNK_SIZE * MESH_SIZE * MESH_SIZE;
    GLuint len = CHUNK_SIZE * MESH_SIZE;
    static glm::vec3* data = new glm::vec3[limit];
    for(GLint i = 0; i < CHUNK_SIZE; i++){
        for(GLint j = 0; j < MESH_SIZE; j++){
            for(GLint k = 0; k < CHUNK_SIZE; k++){
                memcpy(data + p, chunk[i][k]->normal[th][j], sizeof(glm::vec3) * (MESH_SIZE));
                p += MESH_SIZE;
            }
        }
    }
    for(GLint i = 0; i < limit; i++){
        data[i] = data[i] * 0.5f + 0.5f;
    }
    return ResourceManager::MakeTexture(len, len, GL_RGB, data, "NormalMap" + std::to_string(th));
}

Texture2D Scene::Generate_pNormalMap() {
    GLint p = 0;
    GLint limit = CHUNK_SIZE * CHUNK_SIZE * MESH_SIZE * MESH_SIZE + CHUNK_SIZE * MESH_SIZE * 2 + 1;
    GLuint len = CHUNK_SIZE * MESH_SIZE + 1;
    static auto data = new glm::vec3[limit];
    for(GLint i = 0; i < CHUNK_SIZE; i++){
        for(GLint j = 0; j < MESH_SIZE; j++){
            for(GLint k = 0; k < CHUNK_SIZE; k++){
                memcpy(data + p, chunk[i][k]->pnormal[j], sizeof(glm::vec3) * MESH_SIZE);
                p += MESH_SIZE;
            }
            data[p++] = glm::vec3(0.0f);
        }
    }
    for(GLint i = 0; i < CHUNK_SIZE; i++){
        memset(data + p, 0, sizeof(glm::vec3) * MESH_SIZE);
        p += MESH_SIZE;
    }
    data[p++] = glm::vec3(0.0f);
    for(GLint i = 0; i < limit; i++){
        data[i] = data[i] * 0.5f + 0.5f;
    }
    return ResourceManager::MakeTexture(len, len, GL_RGB, data, "pNormalMap");
}