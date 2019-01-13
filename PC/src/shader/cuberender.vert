#version 330 core

layout (location = 0) in vec3 vertex;

uniform mat4 PVMatrix;
uniform mat4 model;

void main(){
    gl_Position = PVMatrix * model * vec4(vertex, 1.0f);
}
