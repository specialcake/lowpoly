#version 330 core
out vec4 FragColor;

in float height;
in vec3 Normal;
in vec3 FragPos;
in vec3 aFragColor;

//uniform vec3 land_color;

void main() {
//    float ambientStrength = 0.8;
//    vec3 lightPos = vec3(1.0f, 4.0f, -1.0f);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diffuseStrength = max(dot(Normal, lightDir), 0.0);
//
//    FragColor = vec4(Normal, 1.0f);
//    FragColor = vec4(land_color * (ambientStrength + diffuseStrength), 1.0f);
    FragColor = vec4(aFragColor, 1.0f);
//    FragColor = vec4(0.0f, height * 0.5f + 0.5f, 0.0f, 1.0f);
//	FragColor = vec4(0.0f, height, 0.0f, 1.0f);
//	FragColor = vec4(1.0f);
}
