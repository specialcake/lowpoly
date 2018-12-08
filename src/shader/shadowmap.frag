#version 330 core

in GS_OUT {
    vec3 Normal;
} fs_in;

uniform vec3 lightdir;

void main() {
    if(dot(fs_in.Normal, lightdir) < 0.0f)
        discard;
}