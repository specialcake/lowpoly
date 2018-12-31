#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 Vertex;

void main() {
	FragColor = vec4(Color, 1.0f);
//	FragColor = vec4(Vertex, 0.0f, 1.0f);
}
