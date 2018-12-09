#version 330 core

in GS_OUT {
    vec3 Normal;
} fs_in;

uniform vec3 lightdir;
uniform float near_plane;
uniform float far_plane;

void main() {
    if(dot(fs_in.Normal, lightdir) < 0.0f)
        discard;
    gl_FragDepth = (gl_FragCoord.z - near_plane) / (far_plane - near_plane);
}