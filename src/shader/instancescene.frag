#version 330 core
out vec4 FragColor;

in GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 Color;
    vec4 FragPosLightSpace;
} fs_in;

struct ParallelLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 1

uniform vec3 viewPos;

uniform ParallelLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D ShadowMap;

vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
float ShadowCalculation(vec4 fragPosLightSpace);

void main() {
    float shadoweffect = ShadowCalculation(fs_in.FragPosLightSpace);
    shadoweffect *= 0.90f;

    //Light calculate
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 factory = CalcParallelLight(dirLight, fs_in.Normal, viewDir, shadoweffect);
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//        factory += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, shadoweffect);

    vec3 aFragColor = fs_in.Color * factory;
    FragColor = vec4(aFragColor, 1.0f);

//    if(shadoweffect > 0.0f) FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//    if(dot(fs_in.Normal, normalize(dirLight.direction)) < 0.0f) FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
//    if(shadoweffect > 0.0f && dot(fs_in.Normal, normalize(dirLight.direction)) < 0.0f)
//        FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

float ShadowCalculation(vec4 fragPosLightSpace){
//    return 0.0f;
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0f)
        return 0.0f;

    float bias = max(0.0008 * dot(fs_in.Normal, lightDir), 0.00055);
//    bias = 0.0f;
//    bias = 0.005f;
//bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth + bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 25.0;
//    float closestDepth = texture(ShadowMap, projCoords.xy).r;
//    shadow = currentDepth + bias > closestDepth ? 1.0 : 0.0;
    shadow = pow(shadow, 0.25f);
    return shadow;
}
vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir, float shadow){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    return ambient + diffuse * (1.0f - shadow);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    ambient *= attenuation;
    diffuse *= attenuation;
    return ambient + diffuse * (1.0f - shadow);
}
