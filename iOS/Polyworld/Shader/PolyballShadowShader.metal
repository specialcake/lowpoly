//
//  PolyballShadowShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/14.
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

struct PolyballShadowVertexIn {
    float3 position  [[attribute(0)]];
    float3 normal    [[attribute(1)]];
    float3 tangent   [[attribute(2)]];
    float2 texCoords [[attribute(3)]];
};

struct PolyballShadowVertexOut {
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

vertex PolyballShadowVertexOut polyballShadowVertex(PolyballShadowVertexIn in [[stage_in]],
                                constant Uniforms &uniforms [[buffer(vertexBufferIndexUniforms)]],
                                uint vid [[vertex_id]])
{
    PolyballShadowVertexOut out;
    
    float4 pos = float4(in.position, 1.0);
    out.position = uniforms.modelViewProjectionMatrix * pos;
    out.position.z = out.position.z / 2 + 0.5;
//    out.position.w = 1.0;
    return out;
}

fragment half4 polyballShadowFragment(PolyballShadowVertexOut in  [[stage_in]],
                                constant Uniforms &uniforms      [[buffer(fragmentBufferIndexUniforms)]],
                                texture2d<float> baseColorMap    [[texture(textureIndexBaseColor)]],
                                texture2d<float> metallicMap     [[texture(textureIndexMetallic)]],
                                texture2d<float> roughnessMap    [[texture(textureIndexRoughness)]],
                                texture2d<float> normalMap       [[texture(textureIndexNormal)]],
                                texture2d<float> emissiveMap     [[texture(textureIndexEmissive)]],
                                texturecube<float> irradianceMap [[texture(textureIndexIrradiance)]])
{
    return half4(0, 0, 0, 1.0f);
}
