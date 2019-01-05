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

uniform sampler2D Cloud;

void main() {
    vec3 color = vec3(0.458824f, 0.780392f, 1.0f);
    FragColor = vec4(color * (Pos.y + 0.3), 1.0f);

    vec3 tmpos = normalize(Pos * 2.0 - 1.0);
    vec2 sphereTexCoords = normalize(tmpos.xz) * cos(asin(tmpos.y)) * 0.5 + 0.5;

    if(Type == 0.0f){
        FragColor = texture(Right, TexCoords.zy);
        if(TexCoords.y > 0.5f){
            float Factory = texture(Cloud, sphereTexCoords).r;
            FragColor = mix(FragColor, vec4(1.0f, 1.0f, 1.0f, 1.0f), Factory);
        }
    }
    else if(Type == 1.0f){
        FragColor = texture(Left, TexCoords.zy);
        if(TexCoords.y > 0.5f){
            float Factory = texture(Cloud, sphereTexCoords).r;
            FragColor = mix(FragColor, vec4(1.0f, 1.0f, 1.0f, 1.0f), Factory);
        }
    }
    else if(Type == 2.0f){
        float Factory = texture(Cloud, sphereTexCoords).r;
        FragColor = mix(texture(Top, TexCoords.xz), vec4(1.0f, 1.0f, 1.0f, 1.0f), Factory);
    }
    else if(Type == 3.0f)
        FragColor = texture(Bottom, TexCoords.xz);
    else if(Type == 4.0f){
        FragColor = texture(Front, TexCoords.xy);
        if(TexCoords.y > 0.5f){
            float Factory = texture(Cloud, sphereTexCoords).r;
            FragColor = mix(FragColor, vec4(1.0f, 1.0f, 1.0f, 1.0f), Factory);
        }
    }
    else{
        FragColor = texture(Back, TexCoords.xy);
        if(TexCoords.y > 0.5f){
            float Factory = texture(Cloud, sphereTexCoords).r;
            FragColor = mix(FragColor, vec4(1.0f, 1.0f, 1.0f, 1.0f), Factory);
        }
    }
//    FragColor = vec4(sphereTexCoords, 0.0f, 1.0f);
}
