#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

uniform mat4 model;
uniform mat4 PVMatrix;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vec4 Pos = PVMatrix * model * vec4(aPos, 1.0);
    gl_Position = Pos.xyww;
}