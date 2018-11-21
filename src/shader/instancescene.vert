#version 330 core
layout (location = 0) in vec2 aVertex;

out float height;

uniform mat4 PVMatrix;
uniform float scalefactor;
uniform vec3 scene_offset;
uniform sampler2D HeightMap;
void main() {
    int idx = gl_InstanceID / 25, idy = gl_InstanceID % 25;
    float deltax = idx - 12.5f, deltay = idy - 12.5f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y) / 25.0f + 1.0f / 50.0f, (idx + aVertex.x) / 25.0f + 1.0f / 50.0f)).r;
    height = pos.y;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
}
