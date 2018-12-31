#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec3 aColor;

out vec3 Color;
out vec2 Vertex;

uniform mat4 model;

void main() {
    Vertex = aVertex;
    Color = aColor;
    gl_Position = model * vec4(aVertex, 0.0f, 1.0f);
}
