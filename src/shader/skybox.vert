#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 Pos;

uniform mat4 PVMatrix;

void main() {
    vec4 pos = PVMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

    Pos = aPos * 0.5f + 0.5f;
}
