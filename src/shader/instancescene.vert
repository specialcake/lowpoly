#version 330 core
layout (location = 0) in vec2 aVertex;

out float height;

uniform mat4 PVMatrix;
uniform float scalefactor;
uniform vec3 scene_offset;
uniform sampler2D HeightMap;
uniform int scene_size;
void main() {
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y) / scene_size + 0.5f / scene_size, (idx + aVertex.x) / scene_size + 0.5f / scene_size)).r;
    height = pos.y;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
}
