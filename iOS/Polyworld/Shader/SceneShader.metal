//
//  Shader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct SceneVertexIn {
    packed_float3 position;
};

struct SceneVertexOut {
    float4 position [[position]];
    float3 fragPosition;
};

struct SceneVertexUniform {
    float4x4 model;
    float4x4 PVMatrix;
};

struct SceneFragmentUniform {
    packed_float3 land_color;
};

vertex SceneVertexOut sceneVertex(constant SceneVertexIn *vertices [[buffer(0)]],
                           constant SceneVertexUniform &uniforms [[buffer(1)]],
                           uint vid [[vertex_id]])
{
    SceneVertexIn in = vertices[vid];
    SceneVertexOut out;
    
    float4x4 model = uniforms.model;
    float4x4 PVMatrix = uniforms.PVMatrix;
    
    out.position = PVMatrix * model * float4(in.position, 1.0);
    out.fragPosition = float3(model * float4(in.position, 1.0));
    return out;
}

fragment float4 sceneFragment(SceneVertexOut vert [[stage_in]],
                               constant SceneFragmentUniform &uniforms [[buffer(0)]])
{
    return float4(vert.fragPosition.y * uniforms.land_color, 1.0);
}
