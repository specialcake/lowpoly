#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform int texture_diffuse1_exist;
uniform vec4 texture_diffuse1_color;
uniform sampler2D texture_diffuse1;

void main(){
    const vec3 lightdir1 = vec3(1.0f, 1.0f, 1.0f);
    const vec3 lightdir2 = vec3(-1.0f, 0.0f, 0.0f);
    float diff1 = max(dot(normalize(Normal), lightdir1), 0.0);
    float diff2 = max(dot(normalize(Normal), lightdir2), 0.0);
    diff2 = 0.0f;

    vec4 diffuse_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if(texture_diffuse1_exist == 0) // pure color
        diffuse_color = texture_diffuse1_color;
    else //texture
        diffuse_color = texture(texture_diffuse1, TexCoords);

    FragColor = vec4((diff1 + diff2) * diffuse_color.rgb, 1.0f);
//    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}