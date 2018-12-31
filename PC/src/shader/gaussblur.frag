#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
}fs_in;
#define sigma 3.0f
#define pi 3.1415926535

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec3 incrementalGaussian = vec3(0.0f);
    incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
    incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

    vec4 avgValue = vec4(0.0f);
    float coefficientSum = 0.0f;

    avgValue += texture(image, fs_in.TexCoords) * incrementalGaussian.x;
    coefficientSum += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;

    vec2 tex_offset = 1.0 / textureSize(image, 0);
    vec2 delta = vec2(0.0f, 0.0f);
    if(horizontal) delta = vec2(1.0f, 0.0f);
    else delta = vec2(0.0f, 1.0f);


    for(int i = 1; i < 3; i++){
        avgValue += texture(image, fs_in.TexCoords - i * tex_offset * delta) * incrementalGaussian.x;
        avgValue += texture(image, fs_in.TexCoords + i * tex_offset * delta) * incrementalGaussian.x;
        coefficientSum += 2.0 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }

    FragColor = avgValue / coefficientSum;
}
