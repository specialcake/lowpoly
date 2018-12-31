#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
//out vec2 debuginfo;

uniform mat4 PVMatrix;
uniform mat4 model;
void main(){
    TexCoords = vertex.zw;
//    debuginfo = vec2(model * vec4(vertex.xy, 0.0f, 1.0f));
    gl_Position = PVMatrix * model * vec4(vertex.xy, 0.0, 1.0);
//    gl_Position = vec4(vertex.xy, -1.0, 1.0);
}
