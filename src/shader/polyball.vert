#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 PVMatrix;
uniform mat4 lightSpaceMatrix;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));

    gl_Position = PVMatrix * vec4(vs_out.FragPos, 1.0f);

    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
}
