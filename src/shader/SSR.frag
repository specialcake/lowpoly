#version 330 core

out vec4 FragColor;

in vec2 FragPos;
in vec2 TexCoords;

uniform float near;
uniform float far;

uniform sampler2D DepthMpa;
uniform sampler2D NormalMap;
uniform sampler2D ColorMap;

void main() {
	FragColor = texture(ColorMap, TexCoords);
}
