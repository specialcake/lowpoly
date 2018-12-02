#version 330 core
layout (location = 0) in vec2 aVertex;

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
out vec3 aFragColor;
#define NR_POINT_LIGHTS 1

uniform int scene_size;
uniform float scalefactor;
uniform float water_height;
uniform vec3 scene_offset;

uniform vec3 water_color;

uniform vec3 viewPos;
uniform mat4 PVMatrix;

uniform ParallelLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = water_height;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
    //normal calc
    vec3 Normal = vec3(0.0f, 1.0f, 0.0f);
    //world coordinate
    vec3 FragPos = pos + scene_offset;
    //Light calculate
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 factory = CalcParallelLight(dirLight, Normal, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        factory += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);

    aFragColor = water_color * factory;
}
vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    return ambient + diffuse;
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
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
    return (ambient + diffuse);
}