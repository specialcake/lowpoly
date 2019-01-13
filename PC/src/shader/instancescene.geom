#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 FragPos;
    vec3 viewPos;
//    vec3 Normal;
    vec3 Color;
    vec4 FragPosLightSpace;
} gs_in[];

out GS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec3 viewNormal;
    vec3 Color;
    vec4 FragPosLightSpace;
} gs_out;

vec3 GetNormal() {
    vec3 a = vec3(gs_in[0].FragPos) - vec3(gs_in[1].FragPos);
    vec3 b = vec3(gs_in[2].FragPos) - vec3(gs_in[1].FragPos);
    return -normalize(cross(a, b));
}
vec3 GetViewNormal(){
    vec3 a = vec3(gs_in[0].viewPos) - vec3(gs_in[1].viewPos);
    vec3 b = vec3(gs_in[2].viewPos) - vec3(gs_in[1].viewPos);
    return -normalize(cross(a, b));
}

void main() {
    vec3 normal = GetNormal();
    vec3 viewnormal = GetViewNormal();

    gl_Position = gl_in[0].gl_Position;
    gs_out.FragPos = gs_in[0].FragPos;
    gs_out.Color = gs_in[0].Color;
    gs_out.Normal = normal;
    gs_out.viewNormal = viewnormal;
    gs_out.FragPosLightSpace = gs_in[0].FragPosLightSpace;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gs_out.FragPos = gs_in[1].FragPos;
    gs_out.Color = gs_in[1].Color;
    gs_out.Normal = normal;
    gs_out.viewNormal = viewnormal;
    gs_out.FragPosLightSpace = gs_in[1].FragPosLightSpace;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gs_out.FragPos = gs_in[2].FragPos;
    gs_out.Color = gs_in[2].Color;
    gs_out.Normal = normal;
    gs_out.viewNormal = viewnormal;
    gs_out.FragPosLightSpace = gs_in[2].FragPosLightSpace;
    EmitVertex();
    EndPrimitive();
}
