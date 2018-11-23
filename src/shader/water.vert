#version 330 core
layout (location = 0) in vec3 aVertex;

//out float height;

uniform mat4 PVMatrix;
uniform float scalefactor;
uniform float water_height;
uniform vec3 meshoffset;
void main() {
    vec3 pos = aVertex * scalefactor + meshoffset;
    pos.y = water_height;
    gl_Position = PVMatrix * vec4(pos, 1.0f);
}
