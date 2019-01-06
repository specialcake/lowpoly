#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 viewNormal;

in GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 viewNormal;
    vec3 Color;
    vec4 FragPosLightSpace;
} fs_in;

struct ParallelLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform float near_plane;
uniform float far_plane;

uniform ParallelLight dirLight;

uniform sampler2D BlurShadow;

vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir, float visibility);
float ShadowCalculation(vec4 fragPosLightSpace);

void main() {
    float visibility = ShadowCalculation(fs_in.FragPosLightSpace);

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 factory = CalcParallelLight(dirLight, fs_in.Normal, viewDir, visibility);

    vec3 aFragColor = fs_in.Color * factory;
    FragColor = vec4(aFragColor, 1.0f);

    viewNormal = vec4(fs_in.viewNormal, 1.0f);
}

float ShadowCalculation(vec4 fragPosLightSpace){
//    return 0.0f;
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0f)
        return 1.0f;

    float bias = max(0.008 * dot(fs_in.Normal, lightDir), 0.0055);
    bias = 0.00051f;
//    bias = 0.0f;
    float currentDepth = projCoords.z;
    float closestDepth = texture(BlurShadow, projCoords.xy).r;
//    shadow = currentDepth + bias > closestDepth ? 1.0 : 0.0;
    float shadow = clamp(exp(-20.0f * (currentDepth - closestDepth - bias)), 0.0, 1.0);
//    return (currentDepth - closestDepth - bias);
    return shadow;
}
vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir, float visibility){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    return ambient + diffuse * visibility;
}