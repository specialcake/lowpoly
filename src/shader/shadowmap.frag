#version 330 core

in GS_OUT {
    vec3 Normal;
    vec4 wCoords;
} fs_in;

uniform vec3 lightdir;
uniform float near_plane;
uniform float far_plane;

void main() {
//    if(dot(fs_in.Normal, lightdir) < 0.0f)
//        discard;
    gl_FragDepth = (-fs_in.wCoords.z - near_plane) / (far_plane - near_plane);
//    if(fs_in.wCoords.z < -40.0f) gl_FragDepth = 1.0f;
//    gl_FragDepth = 0.5f;
}