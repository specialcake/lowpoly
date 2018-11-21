#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
void main() {
    TexCoords = vertex.zw;

    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
}
