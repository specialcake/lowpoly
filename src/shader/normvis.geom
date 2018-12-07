#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 FragPos;
    mat4 PVMatrix;
} gs_in[];

const float MAGNITUDE = 0.2;

vec3 GetNormal() {
    vec3 a = vec3(gs_in[0].FragPos) - vec3(gs_in[1].FragPos);
    vec3 b = vec3(gs_in[2].FragPos) - vec3(gs_in[1].FragPos);
    return -normalize(cross(a, b));
}

void main() {
//    GenerateLine(0); // first vertex normal
//    GenerateLine(1); // second vertex normal
//    GenerateLine(2); // third vertex normal
    vec3 normal = GetNormal();

    vec3 center = (gs_in[0].FragPos + gs_in[1].FragPos + gs_in[2].FragPos) / 3.0f;

    gl_Position = gs_in[0].PVMatrix * vec4(center, 1.0f);
    EmitVertex();
    gl_Position = gs_in[0].PVMatrix * (vec4(center, 1.0f) + vec4(normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}