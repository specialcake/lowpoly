#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in float aTriangleType;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform int scene_size;
uniform float scalefactor;
uniform vec3 scene_offset;

uniform mat4 PVMatrix;
uniform mat4 projection;
uniform mat4 view;

uniform sampler2D HeightMap;
uniform sampler2D NormalMap0;
uniform sampler2D NormalMap1;
uniform sampler2D pNormalMap;

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f))).r;
    float height = pos.y = pos.y * 10.0f;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
    //normal calc
    vec2 NormalTexCoord = vec2((idy + 0.5f) / scene_size, (idx + 0.5f) / scene_size);
    vec3 Normal = vec3(1.0f, 1.0f, 1.0f);
    if(aTriangleType > 0.0f)
        Normal = texture(NormalMap0, NormalTexCoord).rgb;
    else
        Normal = texture(NormalMap1, NormalTexCoord).rgb;
    Normal = (Normal - 0.5f) * 2.0f;
    Normal = vec3(projection * view * vec4(Normal, 0.0f));
    //point normal calc
    vec2 pNormalTexCoord = vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f));
    vec3 pNormal = texture(pNormalMap, pNormalTexCoord).rgb;
    pNormal = (pNormal - 0.5f) * 2.0f;
    pNormal = vec3(projection * view * vec4(pNormal, 0.0f));
    vs_out.normal = pNormal;

    vs_out.normal = Normal;
//    vs_out.normal = vec3(projection * view * vec4(vec3(0.0f, 1.0f, 0.0f), 0.0f));
//    vs_out.normal = vec3(pNormalTexCoord.x, 1.0f, pNormalTexCoord.y);
}

