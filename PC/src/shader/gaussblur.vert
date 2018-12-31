#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

out VS_OUT{
    vec2 TexCoords;
}vs_out;

void main() {
    gl_Position = vec4(aPosition, 0.0f, 1.0f);
    vs_out.TexCoords = aTexCoords;
}