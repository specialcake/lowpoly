#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
    vec3 FragPos;
    vec4 FragPosLightSpace;
}vs_out;

uniform vec3 offset;
uniform vec3 center;
uniform vec4 RotQuat;
uniform mat4 model;
uniform mat4 PVMatrix;
uniform mat4 lightSpaceMatrix;

vec3 Rotater(vec4 Quat, vec4 Pos);

void main() {
//    vec3 rotPos = aPos;
    vec3 rotPos = aPos * vec3(0.3f);
    rotPos -= center;
    rotPos = Rotater(RotQuat, vec4(0.0f, rotPos));
    rotPos += center;

    vs_out.FragPos = rotPos + offset;

    gl_Position = PVMatrix * vec4(vs_out.FragPos, 1.0f);

    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
}

vec3 Rotater(vec4 Quat, vec4 Pos){
    float a = Quat.x, b = Quat.y, c = Quat.z, d = Quat.w;
    vec3 loc1, loc2, loc3;
    loc1.x = 1.0 - 2.0 * c * c - 2.0 * d * d;
    loc1.y = 2.0 * b * c - 2.0 * a * d;
    loc1.z = 2.0 * a * c + 2.0 * b * d;
    loc2.x = 2.0 * b * c + 2.0 * a * d;
    loc2.y = 1.0 - 2.0 * b * b - 2.0 * d * d;
    loc2.z = 2.0 * c * d - 2.0f * a * b;
    loc3.x = 2.0 * b * d - 2.0 * a * c;
    loc3.y = 2.0 * a * b + 2.0 * c * d;
    loc3.z = 1.0 - 2.0 * b * b - 2.0 * c * c;

    vec3 ret;
    ret.x = dot(loc1, Pos.yzw);
    ret.y = dot(loc2, Pos.yzw);
    ret.z = dot(loc3, Pos.yzw);
    return ret;
}
