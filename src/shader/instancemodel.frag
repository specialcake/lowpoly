#version 330 core
out vec4 FragColor;

in vec3 Normal;

uniform vec4 texture_diffuse1_color;

void main(){
    const vec3 lightdir1 = vec3(1.0f, 1.0f, 1.0f);
    float diff1 = max(dot(normalize(Normal), lightdir1), 0.0);

    vec4 diffuse_color = texture_diffuse1_color;

    FragColor = vec4(diff1 * diffuse_color.rgb, 1.0f);
//    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}