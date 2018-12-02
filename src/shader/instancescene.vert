#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in float aTriangleType;

out float height;
out vec3 normal;
//out vec2 NormalTexCoord;
//out float TriangleType;

uniform mat4 PVMatrix;
uniform float scalefactor;
uniform vec3 scene_offset;
uniform sampler2D HeightMap;
uniform sampler2D NormalMap0;
uniform sampler2D NormalMap1;
uniform int scene_size;
void main() {
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f))).r;
    pos.y *= 10.0f;
    height = pos.y;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);

    vec2 NormalTexCoord = vec2((idy + 0.5f) / scene_size, (idx + 0.5f) / scene_size);
//    TriangleType = aTriangleType;
    normal = vec3(1.0f, 1.0f, 1.0f);
    if(aTriangleType > 0.0f)
        normal = texture(NormalMap0, NormalTexCoord).rgb;
    else
        normal = texture(NormalMap1, NormalTexCoord).rgb;
    normal = (normal - 0.5f) * 2.0f;
}
