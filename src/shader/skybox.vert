#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aType;

out float Type;
out vec3 Pos;
out vec3 TexCoords;

uniform mat4 PVMatrix;

void main() {
    vec4 pos = PVMatrix * vec4(aPos.x, aPos.y - 0.1f, aPos.z, 1.0);
    gl_Position = pos.xyww;

    Pos = aPos * 0.5f + 0.5f;
    TexCoords = Pos;
    Type = aType;
}
