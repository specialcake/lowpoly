#version 330 core
layout (location = 0) in vec3 aVertex;

out vec3 Vertex;

uniform mat4 PVMatrix;
uniform mat4 model;
void main() {
    Vertex = vec3(model * vec4(aVertex, 1.0f));
    gl_Position = PVMatrix * model * vec4(aVertex, 1.0f);
//    gl_Position = gl_Position.zyxw;
}
