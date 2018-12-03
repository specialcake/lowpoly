#version 330 core
out vec4 FragColor;

in vec3 aFragColor;

void main() {

    FragColor = vec4(aFragColor, 1.0f);
}
