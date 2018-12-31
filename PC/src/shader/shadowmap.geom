#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 FragPos;
    vec4 wCoords;
} gs_in[];

out GS_OUT{
    vec3 Normal;
    vec4 wCoords;
} gs_out;

vec3 GetNormal() {
    vec3 a = vec3(gs_in[0].FragPos) - vec3(gs_in[1].FragPos);
    vec3 b = vec3(gs_in[2].FragPos) - vec3(gs_in[1].FragPos);
    return -normalize(cross(a, b));
}

void main() {
    vec3 normal = GetNormal();

    gl_Position = gl_in[0].gl_Position;
    gs_out.Normal = normal;
    gs_out.wCoords = gs_in[0].wCoords;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gs_out.Normal = normal;
    gs_out.wCoords = gs_in[1].wCoords;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gs_out.Normal = normal;
    gs_out.wCoords = gs_in[2].wCoords;
    EmitVertex();
    EndPrimitive();
}
