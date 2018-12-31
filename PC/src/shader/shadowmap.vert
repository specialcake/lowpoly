#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in float aTriangleType;

out VS_OUT {
    vec3 FragPos;
    vec4 wCoords;
} vs_out;

uniform int scene_size;
uniform float scalefactor;
uniform vec3 scene_offset;

uniform mat4 PVMatrix;
uniform mat4 view;

uniform sampler2D HeightMap;

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f))).r;
    float height = pos.y = pos.y * 10.0f;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);

    vs_out.FragPos = pos + scene_offset;
    vs_out.wCoords = view * vec4(pos + scene_offset, 1.0f);
}
