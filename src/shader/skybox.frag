#version 330 core
out vec4 FragColor;

in vec3 Pos;

void main() {
    vec3 color = vec3(0.458824f, 0.780392f, 1.0f);
    FragColor = vec4(color * (Pos.y + 0.3), 1.0f);
}
