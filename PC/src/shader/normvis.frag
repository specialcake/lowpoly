#version 330 core
out vec4 FragColor;

in GS_OUT{
    vec4 Color;
} fs_in;

void main() {
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = fs_in.Color;
}
