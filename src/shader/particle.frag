#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform float alpha;

void main() {
    color = texture(sprite, TexCoords);
    if(color.r < 0.8) discard;
    color = vec4(0x82 / 256.0f, 0x38 / 256.0f, 0x03 / 256.0f, alpha);
}