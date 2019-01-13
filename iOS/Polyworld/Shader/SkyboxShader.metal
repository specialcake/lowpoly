//
//  SkyboxShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/5.
//  Copyright © 2019年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct SkyboxVertexIn {
    packed_float3 position;
    float type;
};

struct SkyboxVertexOut {
    float4 position [[position]];
    float3 fragPosition;
    float type;
    float3 texCoords;
};

struct SkyboxVertexUniform {
    float4x4 PVMatrix;
};

vertex SkyboxVertexOut skyboxVertex(constant SkyboxVertexIn *vertices [[buffer(0)]],
                                  constant SkyboxVertexUniform &uniforms [[buffer(1)]],
                                  uint vid [[vertex_id]])
{
    SkyboxVertexIn in = vertices[vid];
    SkyboxVertexOut out;
    float scale = 100;
    float4x4 PVMatrix = uniforms.PVMatrix;
    float4 pos = float4(in.position.x * scale, (in.position.y - 0.2) * scale, in.position.z * scale, 1.0);
    pos = PVMatrix * pos;
    
    out.position = pos;
    out.fragPosition = in.position * 0.5f + 0.5f;
    out.type = in.type;
    out.texCoords = out.fragPosition;
    return out;
}

fragment float4 skyboxFragment(SkyboxVertexOut vert [[stage_in]],
                               texture2d<float>  Front    [[texture(0)]],
                               texture2d<float>  Back     [[texture(1)]],
                               texture2d<float>  Right    [[texture(2)]],
                               texture2d<float>  Left     [[texture(3)]],
                               texture2d<float>  Top      [[texture(4)]],
                               texture2d<float>  Bottom   [[texture(5)]],
                               sampler           sampler2D [[sampler(0)]]
                              )
{
    float eps = 1e-3;
    float4 FragColor = float4(1.0);
    if (abs(vert.type - 0.0) < eps)
        FragColor = Right.sample(sampler2D, vert.texCoords.zy);
    else if (abs(vert.type - 1.0) < eps)
        FragColor = Left.sample(sampler2D, vert.texCoords.zy);
    else if (abs(vert.type - 2.0) < eps)
        FragColor = Top.sample(sampler2D, vert.texCoords.xz);
    else if (abs(vert.type - 3.0) < eps)
        FragColor = Bottom.sample(sampler2D, vert.texCoords.xz);
    else if (abs(vert.type - 4.0) < eps)
        FragColor = Front.sample(sampler2D, vert.texCoords.xy);
    else
        FragColor = Back.sample(sampler2D, vert.texCoords.xy);
    return FragColor;
}

