#version 330 core
out vec4 FragColor;

in float height;
in vec3 normal;
//in vec2 NormalTexCoord;
//in float TriangleType;

uniform vec3 land_color;

void main() {
    vec3 lightPos = vec3(1.0f, -1.0f, 4.0f);
    float ambientStrength = 0.1;

//    vec3 normal = vec3(1.0f, 1.0f, 1.0f);
//    if(TriangleType > 0.0f)
//        normal = texture(NormalMap0, NormalTexCoord).rgb;
//    else
//        normal = texture(NormalMap1, NormalTexCoord).rgb;
//    normal = (normal - 0.5f) * 2.0f;
//    FragColor = vec4((height + 0.1f) * land_color, 1.0f);
    FragColor = vec4(normal, 1.0f);
//    FragColor = vec4(0.0f, height * 0.5f + 0.5f, 0.0f, 1.0f);
//	FragColor = vec4(0.0f, height, 0.0f, 1.0f);
//	FragColor = vec4(1.0f);
}
