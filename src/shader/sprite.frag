#version 330 core

out vec4 Fragcolor;

in vec2 TexCoords;
//in vec2 debuginfo;

uniform sampler2D image;

void main() {
    float redcolor = texture(image, TexCoords).r;
    Fragcolor = vec4(0.0f, redcolor, 0.0f, 1.0f);
//    Fragcolor = vec4(debuginfo, 0.0, 1.0f);

//    color = vec4(mycolor, 0.0f, 1.0f);
}
