#version 330 core
out vec4 FragColor;

in float height;

uniform vec3 land_color;
void main() {
    FragColor = vec4(height * 2.0f * land_color, 1.0f);
//    FragColor = vec4(0.0f, height * 0.5f + 0.5f, 0.0f, 1.0f);
//	FragColor = vec4(0.0f, height, 0.0f, 1.0f);
//	FragColor = vec4(1.0f);
}
