#version 330 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in float aTriangleType;

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

//out float height;
//out vec3 Normal;
//out vec3 FragPos;
out vec3 aFragColor;
#define NR_POINT_LIGHTS 1

uniform int scene_size;
uniform float scalefactor;
uniform vec3 scene_offset;

uniform vec3 land_color;
uniform vec3 rock_color;

uniform vec3 viewPos;
uniform mat4 PVMatrix;

uniform ParallelLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D HeightMap;
uniform sampler2D NormalMap0;
uniform sampler2D NormalMap1;

vec3 CalcParallelLight(ParallelLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    //realworld coordinate calc
    int idx = gl_InstanceID / scene_size, idy = gl_InstanceID % scene_size;
    float deltax = idx - scene_size / 2.0f, deltay = idy - scene_size / 2.0f;
    vec3 pos = vec3(aVertex.x + deltax, 0.0f, aVertex.y + deltay) * scalefactor;
    pos.y = texture(HeightMap, vec2((idy + aVertex.y + 0.5f) / (scene_size + 1.0f), (idx + aVertex.x + 0.5f) / (scene_size + 1.0f))).r;
    float height = pos.y = pos.y * 10.0f;
    gl_Position = PVMatrix * vec4(pos + scene_offset, 1.0f);
    //normal calc
    vec2 NormalTexCoord = vec2((idy + 0.5f) / scene_size, (idx + 0.5f) / scene_size);
    vec3 Normal = vec3(1.0f, 1.0f, 1.0f);
    if(aTriangleType > 0.0f)
        Normal = texture(NormalMap0, NormalTexCoord).rgb;
    else
        Normal = texture(NormalMap1, NormalTexCoord).rgb;
    Normal = (Normal - 0.5f) * 2.0f;
    //world coordinate
    vec3 FragPos = pos + scene_offset;

    //Light calculate
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 factory = CalcParallelLight(dirLight, Normal, viewDir);
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//        factory += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);

    if(height > 1.0f)
        aFragColor = rock_color * factory;
    else
        aFragColor = land_color * factory;
//    float ambientStrength = 0.5;
//    vec3 lightPos = vec3(1.0f, 4.0f, -1.0f);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    lightDir = normalize(vec3(-1.0f, -4.0f, 1.0f));
//    float diffuseStrength = max(dot(Normal, lightDir), 0.0);
////    diffuseStrength *= 0.1f;
//
//    if(height > 1.0f)
//        aFragColor = rock_color * (ambientStrength + diffuseStrength);
//    else
//        aFragColor = max(height, 0.5) * land_color * (ambientStrength + diffuseStrength);
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
