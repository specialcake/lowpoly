#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in float aTriangleType;

out VS_OUT {
    vec3 FragPos;
    vec3 viewPos;
    vec3 Color;
    vec4 FragPosLightSpace;
} vs_out;

uniform int scene_size;
uniform float scalefactor;
uniform vec3 scene_offset;

uniform vec3 lower_color;
uniform vec3 land_color;
uniform vec3 rock_color;

uniform mat4 view;
uniform mat4 PVMatrix;
uniform mat4 lightSpaceMatrix;

uniform sampler2D HeightMap;

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f))).r;
    float height = pos.y = pos.y * 10.0f;
    if(height < 0.10f) pos.y = height = -0.50f;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);

    vs_out.FragPos = pos + scene_offset;
    vs_out.viewPos = vec3(view * vec4(vs_out.FragPos, 1.0f));

    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

    if(vs_out.FragPos.y > 1.0f) vs_out.Color = rock_color;
    else if(vs_out.FragPos.y > 0.3f) vs_out.Color = land_color;
    else vs_out.Color = lower_color;
}
