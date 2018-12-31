//
//  InstanceSceneShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/31.
//  Copyright © 2018年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct InstanceSceneVertexIn {
    float2 position;
    float triangleType;
};

struct InstanceSceneVertexOut {
    float4 position [[position]];
    float3 fragPosition;
    float3 color;
    float4 fragPosLightSpace;
};

struct InstanceSceneVertexUniform {
    uint scene_size;
    float scalefactor;
    packed_float3 scene_offset;
    
    packed_float3 lower_color;
    packed_float3 land_color;
    packed_float3 rock_color;
    float4x4 PVMatrix;
    float4x4 lightSpaceMatrix;
};

struct InstanceSceneFragmentUniform {

};

vertex InstanceSceneVertexOut instanceSceneVertex(constant InstanceSceneVertexIn *vertices [[buffer(0)]],
                             constant InstanceSceneVertexUniform &uniforms [[buffer(1)]],
                             texture2d<float>  HeightMap [[texture(0)]],
                             sampler           sampler2D [[sampler(0)]],
                             uint vid [[vertex_id]],
                             uint iid [[instance_id]])
{
    InstanceSceneVertexIn in = vertices[vid];
    InstanceSceneVertexOut out;
    
    int idx = iid / uniforms.scene_size, idy = iid % uniforms.scene_size;
    float deltax = idx - uniforms.scene_size / 2.0f, deltay = idy - uniforms.scene_size / 2.0f;
    float3 pos = float3(in.position.x + deltax, 0.0f, in.position.y + deltay) * uniforms.scalefactor;
    
    pos.y = HeightMap.sample(sampler2D,  float2((idy + in.position.y + 0.5f) / (uniforms.scene_size + 1.0f), (idx + in.position.x + 0.5f) / (uniforms.scene_size + 1.0f))).r;
    
    float height = pos.y = pos.y * 10.0f;
    
    out.position = uniforms.PVMatrix * float4(pos + uniforms.scene_offset, 1.0f);
    out.fragPosition = pos + uniforms.scene_offset;
    out.fragPosLightSpace = uniforms.lightSpaceMatrix * float4(out.fragPosition, 1.0);
    if(out.fragPosition.y > 1.0f) out.color = uniforms.rock_color;
    else if(out.fragPosition.y > 0.3f) out.color = uniforms.land_color;
    else out.color = uniforms.lower_color;
    return out;
}

fragment float4 instanceSceneFragment(InstanceSceneVertexOut vert [[stage_in]],
                              constant InstanceSceneFragmentUniform &uniforms [[buffer(1)]])
{
    return float4(vert.color, 1.0);
}
