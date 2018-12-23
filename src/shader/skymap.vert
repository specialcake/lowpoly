#version 330 core

layout (location = 0) in vec3 aPosition;

out VS_OUT{
    vec3 Right;
    vec3 Left;
    vec3 Top;
    vec3 Bottom;
    vec3 Front;
    vec3 Back;
}vs_out;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vec3 tmpos = aPosition;
//    tmpos.y = tmpos.y * 0.5f + 0.5f;

    vs_out.Front  = vec3( tmpos.x,  tmpos.y,  tmpos.z);
    vs_out.Back   = vec3( tmpos.x,  tmpos.y, -tmpos.z);
    vs_out.Right  = vec3(-tmpos.z,  tmpos.y,  tmpos.x);
    vs_out.Left   = vec3( tmpos.z,  tmpos.y,  tmpos.x);
    vs_out.Top    = vec3( tmpos.x, -tmpos.z,  tmpos.y);
    vs_out.Bottom = vec3( tmpos.x,  tmpos.z, -tmpos.y);
}
