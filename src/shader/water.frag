#version 330 core
out vec4 FragColor;

in vec3 aFragColor;

//uniform vec3 water_color;
void main() {
//    FragColor = vec4(water_color, 1.0f);
    FragColor = vec4(aFragColor, 1.0f);
//    FragColor = vec4(0.0f, height * 0.5f + 0.5f, 0.0f, 1.0f);
//	FragColor = vec4(0.0f, height, 0.0f, 1.0f);
//	FragColor = vec4(1.0f);
}
