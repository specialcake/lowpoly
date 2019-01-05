
#include <metal_stdlib>
using namespace metal;

enum {
    textureIndexBaseColor,
    textureIndexMetallic,
    textureIndexRoughness,
    textureIndexNormal,
    textureIndexEmissive,
    textureIndexIrradiance = 9
};

enum {
    vertexBufferIndexUniforms = 1
};

enum {
    fragmentBufferIndexUniforms = 0
};

struct Vertex {
    float3 position  [[attribute(0)]];
    float3 normal    [[attribute(1)]];
    float3 tangent   [[attribute(2)]];
    float2 texCoords [[attribute(3)]];
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoords;
    float3 worldPos;
    float3 normal;
    float3 bitangent;
    float3 tangent;
};

struct Uniforms {
    float4x4 modelMatrix;
    float4x4 modelViewProjectionMatrix;
    float3x3 normalMatrix;
    float3 cameraPos;
    float3 directionalLightInvDirection;
    float3 lightPosition;
};

struct LightingParameters {
    float3 lightDir;
    float3 viewDir;
    float3 halfVector;
    float3 reflectedVector;
    float3 normal;
    float3 reflectedColor;
    float3 irradiatedColor;
    float3 baseColor;
    float3 diffuseLightColor;
    float  NdotH;
    float  NdotV;
    float  NdotL;
    float  HdotL;
    float  metalness;
    float  roughness;
};
    
#define SRGB_ALPHA 0.055

float linear_from_srgb(float x) {
    if (x <= 0.04045)
        return x / 12.92;
    else
        return powr((x + SRGB_ALPHA) / (1.0 + SRGB_ALPHA), 2.4);
}

float3 linear_from_srgb(float3 rgb) {
    return float3(linear_from_srgb(rgb.r), linear_from_srgb(rgb.g), linear_from_srgb(rgb.b));
}

vertex VertexOut sunVertex(Vertex in [[stage_in]],
                             constant Uniforms &uniforms [[buffer(vertexBufferIndexUniforms)]])
{
    VertexOut out;
    float4 pos = uniforms.modelViewProjectionMatrix * float4(in.position, 1.0);;
    out.position = pos; // !
    out.texCoords = in.texCoords;
    out.normal = uniforms.normalMatrix * in.normal;
    out.tangent = uniforms.normalMatrix * in.tangent;
    out.bitangent = uniforms.normalMatrix * cross(in.normal, in.tangent);
    out.worldPos = (uniforms.modelMatrix * float4(in.position, 1.0)).xyz;
    return out;
}

static float3 diffuseTerm(LightingParameters parameters) {
    float3 diffuseColor = (parameters.baseColor.rgb / M_PI_F) * (1.0 - parameters.metalness);
    return diffuseColor * parameters.NdotL * parameters.diffuseLightColor;
}

static float SchlickFresnel(float dotProduct) {
    return pow(clamp(1.0 - dotProduct, 0.0, 1.0), 5.0);
}

static float Geometry(float NdotV, float alphaG) {
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1.0 / (NdotV + sqrt(a + b - a * b));
}

static float TrowbridgeReitzNDF(float NdotH, float roughness) {
    if (roughness >= 1.0)
        return 1.0 / M_PI_F;
    
    float roughnessSqr = roughness * roughness;
    
    float d = (NdotH * roughnessSqr - NdotH) * NdotH + 1;
    return roughnessSqr / (M_PI_F * d * d);
}

static float3 specularTerm(LightingParameters parameters) {
    float specularRoughness = parameters.roughness * (1.0 - parameters.metalness) + parameters.metalness;
    
    float D = TrowbridgeReitzNDF(parameters.NdotH, specularRoughness);
    
    float Cspec0 = 0.04;
    float3 F = mix(Cspec0, 1, SchlickFresnel(parameters.HdotL));
    float alphaG = powr(specularRoughness * 0.5 + 0.5, 2);
    float G = Geometry(parameters.NdotL, alphaG) * Geometry(parameters.NdotV, alphaG);
    
    float3 specularOutput = (D * G * F * parameters.irradiatedColor) * (1.0 + parameters.metalness * parameters.baseColor) +
                                                 parameters.irradiatedColor * parameters.metalness * parameters.baseColor;
    
    return specularOutput;
}

fragment half4 sunFragment(VertexOut in                     [[stage_in]],
                             constant Uniforms &uniforms      [[buffer(fragmentBufferIndexUniforms)]],
                             texture2d<float> baseColorMap    [[texture(textureIndexBaseColor)]],
                             texture2d<float> metallicMap     [[texture(textureIndexMetallic)]],
                             texture2d<float> roughnessMap    [[texture(textureIndexRoughness)]],
                             texture2d<float> normalMap       [[texture(textureIndexNormal)]],
                             texture2d<float> emissiveMap     [[texture(textureIndexEmissive)]],
                             texturecube<float> irradianceMap [[texture(textureIndexIrradiance)]])
{
    float3 sunColor = float3(1.0f, 0.55686f, 0.43529f);
    float ambient = 1.0f;
    float diff = max(dot(normalize(in.normal), normalize(uniforms.directionalLightInvDirection)), 0.0);
    
    return half4(half3((ambient + diff) * sunColor), 1.0f);
}
