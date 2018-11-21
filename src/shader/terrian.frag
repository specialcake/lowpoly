#version 330 core
out vec4 FragColor;

in vec3 Vertex;
void main() {
	FragColor = vec4(Vertex.y * vec3(25.0f / 255.0f, 161.0f / 255.0f, 245.0f / 255.0f), 1.0f);
//	FragColor = vec4(Vertex.xyz, 1.0f);
}
