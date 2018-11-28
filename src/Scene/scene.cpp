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
//            if(i + 1 != CHUNK_SIZE)
//                this->chunk[i][j]->xPos = this->chunk[i + 1][j];
//            if(i != 0)
//                this->chunk[i][j]->xNeg = this->chunk[i - 1][j];
//            if(j + 1 != CHUNK_SIZE)
//                this->chunk[i][j]->zPos = this->chunk[i][j + 1];
//            if(j != 0)
//                this->chunk[i][j]->zNeg = this->chunk[i][j - 1];
        }
    }

//    for(GLint i = 0; i < CHUNK_SIZE; i++) {
//        for(GLint j = 0; j < CHUNK_SIZE; j++) {
//            if(i + 1 != CHUNK_SIZE){
////                this->chunk[i][j]->xPos = this->chunk[i + 1][j];
//                for(GLint k = 0; k < CHUNK_SIZE; k++){
//                    this->chunk[i][j]->submesh[MESH_SIZE - 1][k]->xPos = this->chunk[i][j]->xPos->submesh[0][k];
//                }
//            }
//            if(i != 0){
////                this->chunk[i][j]->xNeg = this->chunk[i - 1][j];
//                for(GLint k = 0; k < CHUNK_SIZE; k++){
//                    this->chunk[i][j]->submesh[0][k]->xNeg = this->chunk[i][j]->xNeg->submesh[MESH_SIZE - 1][k];
//                }
//            }
//            if(j + 1 != CHUNK_SIZE) {
////                this->chunk[i][j]->zPos = this->chunk[i][j + 1];
//                for (GLint k = 0; k < CHUNK_SIZE; k++) {
//                    this->chunk[i][j]->submesh[k][MESH_SIZE - 1]->zPos = this->chunk[i][j]->zPos->submesh[k][0];
//                }
//            }
//            if(j != 0){
////                this->chunk[i][j]->zNeg = this->chunk[i][j - 1];
//                for(GLint k = 0; k < CHUNK_SIZE; k++){
//                    this->chunk[i][j]->submesh[k][0]->zNeg = this->chunk[i][j]->zNeg->submesh[k][MESH_SIZE - 1];
//                }
//            }
//        }
//    }

    this->cur_Chunk = this->chunk[CHUNK_RADIUS][CHUNK_RADIUS];
    this->cur_Submesh = this->cur_Chunk->submesh[MESH_RADIUS][MESH_RADIUS];
    this->InitBuffer();

    this->generator = new Noise(NOISE_SIZE);

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
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
    };
    glGenVertexArrays(1, &this->instanceVAO);
    glGenBuffers(1, &this->instanceVBO);

    glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instance_vertices), &instance_vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->instanceVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Scene::generate_scene() {
    for(GLint i = 0; i < CHUNK_SIZE; i++) {
        for (GLint j = 0; j < CHUNK_SIZE; j++) {
            chunk[i][j]->generate_map();
            chunk[i][j]->generate_water();
        }
    }
    std::cout << this->chunk[0][0]->height[0][0] << std::endl;
    this->chunk[0][0]->height[1][1] = 3.0f;
}
void Scene::draw(glm::mat4 PVMatrix) {
//    this->map_shader.setVec3("land_color", LAND_COLOR);
//    glBindVertexArray(this->VAO);
//    for(GLint i = 0; i < CHUNK_SIZE; i++) {
//        for (GLint j = 0; j < CHUNK_SIZE; j++) {
////            printf("[%d, %d] ", chunk[i][j]->pos_x, chunk[i][j]->pos_z);
//            chunk[i][j]->draw_map();
//        }
////        printf("\n");
//    }
//    glBindVertexArray(0);
    this->map_instance_shader.use();
    HeightMap = this->Generate_HeightMap();
    glActiveTexture(0);
    this->HeightMap.Bind();
    this->map_instance_shader.setMat4("PVMatrix", PVMatrix);
    this->map_instance_shader.setVec3("land_color", LAND_COLOR);
    this->map_instance_shader.setFloat("scalefactor", MESH_LENGTH);
    this->map_instance_shader.setVec3("scene_offset", this->offset);
    this->map_instance_shader.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);

    glBindVertexArray(this->instanceVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);

    this->water_shader.use();
    this->water_shader.setMat4("PVMatrix", PVMatrix);
    this->water_shader.setVec3("water_color", SEA_COLOR);
    this->water_shader.setFloat("scalefactor", MESH_LENGTH);
    this->water_shader.setVec3("scene_offset", this->offset);
    this->water_shader.setInt("scene_size", MESH_SIZE * CHUNK_SIZE);
    this->water_shader.setFloat("water_height", SEA_LEVEL);

    glBindVertexArray(this->VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE);
    glBindVertexArray(0);
//    for(GLint i = 0; i < CHUNK_SIZE; i++) {
//        for (GLint j = 0; j < CHUNK_SIZE; j++) {
//            chunk[i][j]->draw_water();
//        }
//    }
}

void Scene::UpdateChunks() {
    this->debugflag = false;
    Direction dir = ORIGIN_POS;
    GLint Chunkx, Chunkz, Meshx, Meshz;
    GetLocationbyCamera(Chunkx, Chunkz, Meshx, Meshz);
//    printf("(%d, %d) -> (%d, %d)\n", Chunkx, Chunkz, Meshx, Meshz);
    if(Chunkx == cur_Chunk->pos_x && Chunkz == cur_Chunk->pos_z){
        cur_Submesh = cur_Chunk->submesh[Meshx][Meshz];
        return ;
    }
    this->debugflag = true;
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
        }
        dir = MOVE_ZNEG;
    }
    cur_Chunk = this->chunk[CHUNK_RADIUS][CHUNK_RADIUS];
    cur_Submesh = cur_Chunk->submesh[MESH_RADIUS][MESH_RADIUS];
//    UpdateChunk(dir);
}

void Scene::GetLocationbyCamera(GLint &cx, GLint &cz, GLint &mx, GLint &mz) {
    glm::vec3 position = ResourceManager::camera.Position -
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
    GLint len = CHUNK_SIZE * CHUNK_SIZE + 1;
    GLfloat* data = new GLfloat[limit];
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
//    if(this->debugflag) {
//        printf("------------------------------------\n");
//        for(int i = 0; i < len; i++){
//            for(int j = 0; j < len; j++){
//                printf("%.3lf ", data[i * len + j]);
//            }
//            printf("\n");
//        }
//    }
    for(GLint i = 0; i < limit; i++){
        data[i] = data[i] * 0.1;
    }
//    if(this->debugflag) {
//        printf("\n===============================\n");
//    }
    return ResourceManager::MakeTexture(CHUNK_SIZE * MESH_SIZE + 1, CHUNK_SIZE * MESH_SIZE + 1, GL_RED, data, "HeightMap");
}