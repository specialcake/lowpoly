#version 330 core
layout (location = 0) in vec2 aVertex;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

uniform int scene_size;
uniform float scalefactor;
uniform vec3 scene_offset;

uniform float water_height;

uniform mat4 PVMatrix;
uniform mat4 lightSpaceMatrix;

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = water_height;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
    //normal calc
    vs_out.Normal = vec3(0.0f, 1.0f, 0.0f);
    //world coordinate
    vs_out.FragPos = pos + scene_offset;

    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}