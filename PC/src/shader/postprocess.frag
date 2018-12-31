#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D scene;

void main() {
    color = vec4(1.0f);
    color.a = 1.0f;
}