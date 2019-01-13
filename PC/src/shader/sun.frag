#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;

const vec3 sunColor = vec3(1.0f, 0.55686f, 0.43529f);

uniform vec3 lightdir;

void main(){
    float ambient = 1.0f;
    float diff = max(dot(normalize(Normal), normalize(lightdir)), 0.0);

    FragColor = vec4((ambient + diff) * sunColor, 1.0f);

//    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
//    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
//    else BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}