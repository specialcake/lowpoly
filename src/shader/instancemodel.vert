#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 model;

out vec3 Normal;

uniform mat4 PVMatrix;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = PVMatrix * model * vec4(aPos, 1.0);
}