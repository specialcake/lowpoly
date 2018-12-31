#version 330 core
out vec4 FragColor;

in vec3 Vertex;

uniform vec3 land_color;
void main() {
    FragColor = vec4(Vertex.y * land_color, 1.0f);
//    FragColor = vec4(0.0f, (Vertex.y - 0.5) * 2.0, 0.0f, 1.0f);
//	FragColor = vec4(Vertex.xyz, 1.0f);
}
