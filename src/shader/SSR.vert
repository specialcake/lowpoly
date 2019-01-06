#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 FragPos;
out vec2 TexCoords;

void main() {
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	TexCoords = aPos * 0.5f + 0.5f;
	FragPos = aPos;
}
