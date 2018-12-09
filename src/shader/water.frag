#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
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

uniform vec3 water_color;
uniform ParallelLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D ShadowMap;

vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow);
float ShadowCalculation(vec4 fragPosLightSpace);

void main() {
    float shadoweffect = ShadowCalculation(fs_in.FragPosLightSpace);

    //Light calculate
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 factory = CalcParallelLight(dirLight, fs_in.Normal, viewDir, shadoweffect);
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//        factory += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, shadoweffect);

    vec3 aFragColor = vec3(1.0f, 1.0f, 1.0f);

    aFragColor = water_color * factory;

    FragColor = vec4(aFragColor, 1.0f);
}

float ShadowCalculation(vec4 fragPosLightSpace){
//    return 0.0f;
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0f;

    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.0005);
    bias = 0.00033;
    float currentDepth = projCoords.z;
//    currentDepth = (currentDepth - 0.1f) / (200.0f - 0.1f);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    float closestDepth = texture(ShadowMap, projCoords.xy).r;
//    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    shadow = clamp( exp( 80.0f * ( currentDepth - closestDepth ) ), 0.0, 1.0 );
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
