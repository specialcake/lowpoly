#version 330 core

out vec4 Fragcolor;

in vec2 TexCoords;
//in vec2 debuginfo;

uniform sampler2D image;

void main() {
    vec4 color = texture(image, TexCoords);
    Fragcolor = vec4(color.rrr, 1.0f);
//    Fragcolor = color;
//    Fragcolor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    Fragcolor = vec4(vec3(redcolor), 1.0f);
//    Fragcolor = vec4(debuginfo, 0.0, 1.0f);

//    color = vec4(mycolor, 0.0f, 1.0f);
}
