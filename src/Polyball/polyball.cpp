//
// Created by dydxh on 1/8/19.
//

#include "polyball.h"

Polyball::Polyball(Shader shader) {
    Speed = glm::vec3(0.0f, 0.0f, 0.0f);
    SpeedLimit = glm::vec3(0.0f);
    Acceleration = glm::vec3(2.0f, 0.0f, 2.0f);
    Resistance = glm::vec3(1.0f, 3.0f, 1.0f);
    Position = glm::vec3(0.0f, 2.0f, 5.0f);
    Radius = 0.5f * 0.3f;
    Mov.Front = glm::vec3(1.0f, 0.0f, 0.0f);
    Mov.Right = glm::vec3(0.0f, 0.0f, 1.0f);
    Mov.Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    Cam.Front = ResourceManager::camera.Front;
    Cam.Right = ResourceManager::camera.Right;
    Cam.Up    = ResourceManager::camera.Up;

    collision = (Collision){false, glm::vec3(0.0f), glm::vec3(0.0f)};
    rotate_state = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    BallShader = shader;
    Initialize();
}

Polyball::~Polyball() {}

void Polyball::Initialize() {
    modelptr = ResourceManager::GetModel("polyball");
}

glm::vec3 Polyball::GenCameraPosition() {
    const GLfloat distance = 3.0f;
    static GLfloat cosvalue = glm::cos(PI / 12.0);
    static GLfloat sinvalue = glm::sin(PI / 12.0);
    glm::vec3 ret = Position;
    ret += -distance * cosvalue * Cam.Front;
    ret += distance * sinvalue * Cam.Up;
    return ret;
}

void Polyball::Render(glm::mat4 view, glm::mat4 PVMatrix, glm::mat4 lightSpaceMatrix, Texture2D BlurShadow) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    model = glm::scale(model, glm::vec3(0.3f));

    BallShader.use();
    BallShader.setMat4("view", view);
    BallShader.setMat4("PVMatrix", PVMatrix);
    BallShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    BallShader.setMat4("model", model);
    BallShader.setVec4("RotQuat", glm::vec4(rotate_state.w, rotate_state.x, rotate_state.y, rotate_state.z));
    BallShader.setLight(ResourceManager::camera.Position);

    glActiveTexture(GL_TEXTURE0);
    BlurShadow.Bind();

    modelptr->Draw(BallShader);

//    Tools::PrintVec3(Position);
//    Tools::PrintVec3(Speed);
}
glm::vec3 Polyball::CollisionObject(Scene *scene) {
    glm::vec3 delta_pos = glm::vec3(0.0f);
    for(int typenum = 0; typenum < 3; typenum++){
        for(int i = 0; i < scene->Treeplace[typenum].size(); i++){
            if(typenum <= 1){
                Trunk tk = scene->Treeplace[typenum][i].trunk;
                GLfloat seglen = tk.height;
                glm::vec3 seg = glm::vec3(0.0f, 1.0f, 0.0);
                glm::vec3 vect = Position - tk.bottom;
                GLfloat len = glm::dot(seg, vect);
                glm::vec3 Q = len * seg + tk.bottom;
                GLfloat mindist;
                if(len < 0) mindist = glm::length(vect);
                else if(len > seglen) mindist = glm::length(Position - (tk.bottom + glm::vec3(0.0f, tk.height, 0.0f)));
                else mindist = glm::length(Position - Q);
                if(mindist < Radius + tk.Radius){
                    collision.exist = true;
                    glm::vec3 normal = glm::normalize(glm::vec3(Position.x - tk.bottom.x, 0.0f, Position.z - tk.bottom.z));
                    collision.Normal += normal;
                    delta_pos += (Radius + tk.Radius - mindist) * normal;
                }
            }
            else{
                BoundBox box = scene->Treeplace[typenum][i].box;
                glm::vec3 cp = box.Point;// - glm::vec3(box.Left, box.Down, box.Front) / 2.0f;
                if(Tools::distance(Position, cp) >= 1.1) continue;
                GLfloat test = Tools::distance(Position, cp);

                glm::vec3 relpos = Position - cp;
                GLfloat disx = glm::abs(relpos.x) - box.Left / 2.0f;
                GLfloat disy = glm::abs(relpos.y) - box.Down / 2.0f;
                GLfloat disz = glm::abs(relpos.z) - box.Front / 2.0f;
                GLfloat dist = glm::sqrt(glm::pow(glm::max(0.0f, disx), 2) +
                                         glm::pow(glm::max(0.0f, disy), 2) +
                                         glm::pow(glm::max(0.0f, disz), 2));
                if(dist < Radius){
                    collision.exist = true;
                    glm::vec3 normal;
                    if(disx >= disy && disx >= disz){
                        if(relpos.x > 0) normal = glm::vec3(1.0f, 0.0f, 0.0f);
                        else normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                    }
                    else if(disy >= disx && disy >= disz){
                        if(relpos.y > 0) normal = glm::vec3(0.0f, 1.0f, 0.0f);
                        else normal = glm::vec3(0.0f, -1.0f, 0.0f);
                    }
                    else{
                        if(relpos.z > 0) normal = glm::vec3(0.0f, 0.0f, 1.0f);
                        else normal = glm::vec3(0.0f, 0.0f, -1.0f);
                    }
                    collision.Normal += normal;
                    delta_pos += normal * (Radius - dist);
                }
            }
        }
    }
    return delta_pos;
}
void Polyball::CollisionCheck(Scene* scene) {
    collision = (Collision){false, glm::vec3(0.0f), glm::vec3(0.0f)};
//    if(Position.y <= 0.0f){
//        Position.y = 0.0f;
//        collision = (Collision){true, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
//    }
    GLint cx, cy, mx, my;
    GetChunkMeshID(scene, cx, cy, mx, my);
    dcx = cx, dcy = cy, dmx = mx, dmy = my;
    for(int i = -1; i <= 2; i++)
        for(int j = -1; j <= 2; j++){
            int meshx = mx + i, meshy = my + j;
            int chunkx, chunky;
            if(meshx < 0) chunkx = cx - 1, meshx += MESH_SIZE;
            else if(meshx >= MESH_SIZE) chunkx = cx + 1, meshx -= MESH_SIZE;
            else chunkx = cx;
            if(meshy < 0) chunky = cy - 1, meshy += MESH_SIZE;
            else if(meshy >= MESH_SIZE) chunky = cy + 1, meshy -= MESH_SIZE;
            else chunky = cy;
            Maycol[i + 1][j + 1] = scene->chunk_offset[chunkx][chunky] + scene->mesh_offset[meshx][meshy];
            Maycol[i + 1][j + 1].y = scene->chunk[chunkx][chunky]->height[meshx][meshy];
            if(Maycol[i + 1][j + 1].y < 0.1f) Maycol[i + 1][j + 1].y = -0.5f;
        }
    glm::vec3 delta_pos = glm::vec3(0.0f);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            glm::vec3 a, b, c;
            GLfloat dist;
            a = Maycol[i][j];
            b = Maycol[i + 1][j];
            c = Maycol[i + 1][j + 1];
            dist = Tools::distance(Position, a, b, c);
            if(dist <= Radius){
                collision.exist = true;
                glm::vec3 normal = glm::normalize(glm::cross(c - a, b - a));
                collision.Normal += normal;
                delta_pos += (Radius - dist) * normal;
            }
            b = c;
            c = Maycol[i][j + 1];
            dist = Tools::distance(Position, a, b, c);
            if(dist <= Radius){
                collision.exist = true;
                glm::vec3 normal = glm::normalize(glm::cross(c - a, b - a));
                collision.Normal += normal;
                delta_pos += (Radius - dist) * normal;
            }
        }

    }
    delta_pos += CollisionObject(scene);
    if(Position.y < 0.1){
        collision.exist = true;
        collision.Normal += glm::vec3(0.0f, 1.0f, 0.0f);
        delta_pos += (0.1f - Position.y) * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    Position += delta_pos;
    if(collision.exist)
        collision.Normal = glm::normalize(collision.Normal);
}
void Polyball::GetChunkMeshID(Scene* scene, GLint &cx, GLint &cz, GLint &mx, GLint &mz){
    glm::vec3 position = Position -
                         scene->chunk[CHUNK_RADIUS][CHUNK_RADIUS]->submesh[MESH_RADIUS][MESH_RADIUS]->get_Position() -
                         glm::vec3(MESH_LENGTH / 2.0f, 0.0f, MESH_LENGTH / 2.0f);

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

//UpdatePosition => Process Speed
void Polyball::UpdatePosition(float deltaTime, Scene* scene) {
    Position += Speed * deltaTime;
    CollisionCheck(scene);

    GLfloat angle = wspeed * deltaTime / 2.0f;
    glm::quat rot = glm::quat(glm::cos(angle),
                    glm::sin(angle) * rotate_axis.x,
                    glm::sin(angle) * rotate_axis.y,
                    glm::sin(angle) * rotate_axis.z);
    rotate_state = rot * rotate_state;
}

//UpdateSpeed => Process Acceleration
void Polyball::UpdateSpeed(float deltaTime) {
    if(collision.exist){
        GLfloat velocity = -glm::dot(collision.Normal, Speed);
        Speed += velocity * collision.Normal;
        if(velocity > 2.0f)
            Speed += 0.1f * velocity * collision.Normal;
    }
    UpdateMovVec();

    glm::vec3 delta_v = glm::vec3(0.0f);
    bool noKeyboard = true;
    if(collision.exist){
        if(ResourceManager::Keys[GLFW_KEY_W]){
            GLfloat cosvalue = glm::dot(Cam.Front, Mov.Up);
            delta_v += (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_S]){
            GLfloat cosvalue = glm::dot(Cam.Front, Mov.Up);
            delta_v -= (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_D]){
            GLfloat cosvalue = glm::dot(Cam.Right, Mov.Up);
            delta_v += (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_A]){
            GLfloat cosvalue = glm::dot(Cam.Right, Mov.Up);
            delta_v -= (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime;
            noKeyboard = false;
        }
        if(ResourceManager::Keys[GLFW_KEY_SPACE]){
            delta_v += glm::vec3(0.0f, 3.0f, 0.0f);
        }
        //Sliding Friction
        glm::vec3 movXZ = Speed;
        GLfloat movSpeed = glm::length(movXZ);
        if(movSpeed > eps) {
            if (Resistance.x * deltaTime > movSpeed && noKeyboard)
                delta_v -= glm::normalize(movXZ) * movSpeed;
            else
                delta_v -= glm::normalize(movXZ) * Resistance.x * deltaTime;
        }
    }
    //Gravity
    delta_v.y -= Resistance.y * deltaTime;
    Speed += delta_v;

    UpdateMovVec();

    if(glm::length(glm::vec2(Speed.x, Speed.z)) < 0.001) wspeed = 0.0f;
    else wspeed = glm::length(Speed) / Radius;
    rotate_axis = -Mov.Right;
}
void Polyball::UpdateMovVec(){
    if(glm::abs(Speed.x) < eps && glm::abs(Speed.z) < eps){
        Mov.Front = glm::vec3(1.0f, 0.0f, 0.0f);
        Mov.Right = glm::vec3(0.0f, 0.0f, 1.0f);
        Mov.Up    = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else{
        Mov.Front = glm::normalize(Speed);
        Mov.Right = glm::normalize(glm::cross(Mov.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Mov.Up    = glm::normalize(glm::cross(Mov.Right, Mov.Front));
    }
}
