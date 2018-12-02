#version 330 core
out vec4 FragColor;

in float height;
in vec2 NormalTexCoord;
in float TriangleType;

uniform vec3 land_color;
uniform sampler2D NormalMap0;
uniform sampler2D NormalMap1;
void main() {
    vec3 normal = vec3(1.0f, 1.0f, 1.0f);
    if(TriangleType > 0.0f)
        normal = texture(NormalMap0, NormalTexCoord).rgb;
    else
        normal = texture(NormalMap1, NormalTexCoord).rgb;
    FragColor = vec4((height + 0.1f) * land_color, 1.0f);
    FragColor = vec4(normal, 1.0f);
//    FragColor = vec4(0.0f, height * 0.5f + 0.5f, 0.0f, 1.0f);
//	FragColor = vec4(0.0f, height, 0.0f, 1.0f);
//	FragColor = vec4(1.0f);
}
