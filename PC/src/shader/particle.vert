#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 PVMatrix;
uniform vec3 offset;
uniform float scale;

void main() {
    TexCoords = vertex.zw;
//    gl_Position = PVMatrix * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
    gl_Position = PVMatrix * vec4(vertex.x * scale + offset.x, vertex.y * scale + offset.y, offset.z, 1.0);
}
