#version 330 core
out vec4 FragColor;

in float Type;
in vec3 Pos;
in vec3 TexCoords;

//uniform sampler3D skymap;
uniform sampler2D Right;
uniform sampler2D Left;
uniform sampler2D Top;
uniform sampler2D Bottom;
uniform sampler2D Front;
uniform sampler2D Back;

void main() {
    vec3 color = vec3(0.458824f, 0.780392f, 1.0f);
    FragColor = vec4(color * (Pos.y + 0.3), 1.0f);

    if(Type == 0.0f)
        FragColor = texture(Right, TexCoords.zy);
    else if(Type == 1.0f)
        FragColor = texture(Left, TexCoords.zy);
    else if(Type == 2.0f)
        FragColor = texture(Top, TexCoords.xz);
    else if(Type == 3.0f)
        FragColor = texture(Bottom, TexCoords.xz);
    else if(Type == 4.0f)
        FragColor = texture(Front, TexCoords.xy);
    else
        FragColor = texture(Back, TexCoords.xy);
}
