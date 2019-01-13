#version 330 core
out vec4 Fragcolor;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    Fragcolor = vec4(textColor, 1.0) * sampled;
//    Fragcolor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}