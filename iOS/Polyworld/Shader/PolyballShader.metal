//
//  PolyballShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/11.
//  Copyright © 2019年 Sxy. All rights reserved.
//

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

float3 Rotater(float4 Quat, float4 Pos){
    float a = Quat.x, b = Quat.y, c = Quat.z, d = Quat.w;
    float3 loc1, loc2, loc3;
    loc1.x = 1.0 - 2.0 * c * c - 2.0 * d * d;
    loc1.y = 2.0 * b * c - 2.0 * a * d;
    loc1.z = 2.0 * a * c + 2.0 * b * d;
    loc2.x = 2.0 * b * c + 2.0 * a * d;
    loc2.y = 1.0 - 2.0 * b * b - 2.0 * d * d;
    loc2.z = 2.0 * c * d - 2.0f * a * b;
    loc3.x = 2.0 * b * d - 2.0 * a * c;
    loc3.y = 2.0 * a * b + 2.0 * c * d;
    loc3.z = 1.0 - 2.0 * b * b - 2.0 * c * c;
    
    float3 ret;
    ret.x = dot(loc1, Pos.yzw);
    ret.y = dot(loc2, Pos.yzw);
    ret.z = dot(loc3, Pos.yzw);
    return ret;
}
    
vertex VertexOut polyballVertex(Vertex in [[stage_in]],
                           constant Uniforms &uniforms [[buffer(vertexBufferIndexUniforms)]],
                           uint vid [[vertex_id]])
{
    VertexOut out;
    
    float4 pos = float4(in.position, 1.0);
    out.position = uniforms.modelViewProjectionMatrix * pos;
    out.texCoords = in.texCoords;
    out.normal = uniforms.normalMatrix * in.normal;
    out.tangent = uniforms.normalMatrix * in.tangent;
    out.bitangent = uniforms.normalMatrix * cross(in.normal, in.tangent);
    out.worldPos = (uniforms.modelMatrix * pos).xyz;
    return out;
}

fragment half4 polyballFragment(VertexOut in  [[stage_in]],
                           constant Uniforms &uniforms      [[buffer(fragmentBufferIndexUniforms)]],
                           texture2d<float> baseColorMap    [[texture(textureIndexBaseColor)]],
                           texture2d<float> metallicMap     [[texture(textureIndexMetallic)]],
                           texture2d<float> roughnessMap    [[texture(textureIndexRoughness)]],
                           texture2d<float> normalMap       [[texture(textureIndexNormal)]],
                           texture2d<float> emissiveMap     [[texture(textureIndexEmissive)]],
                           texturecube<float> irradianceMap [[texture(textureIndexIrradiance)]])
{
    float3 color = float3(1.0f, 1.0f, 1.0f);
    float ambient = 0.1f;
    float diff = max(dot(normalize(in.normal), normalize(-uniforms.directionalLightInvDirection)), 0.0);
    
    return half4(half3((ambient + diff) * color), 1.0f);
}
