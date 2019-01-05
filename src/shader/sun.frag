#version 330 core
out vec4 FragColor;

in vec3 Normal;

const vec3 sunColor = vec3(1.0f, 0.55686f, 0.43529f);

uniform vec3 lightdir;

void main(){
    float ambient = 1.0f;
    float diff = max(dot(normalize(Normal), normalize(lightdir)), 0.0);

    FragColor = vec4((ambient + diff) * sunColor, 1.0f);
//    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}