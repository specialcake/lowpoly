//
//  InstanceSceneShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/31.
//  Copyright © 2018年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct ParallelLight{
    packed_float3 direction;
    packed_float3 ambient;
    packed_float3 diffuse;
    packed_float3 specular;
};

struct InstanceSceneVertexIn {
    packed_float2 position;
    float triangleType;
};

struct InstanceSceneVertexOut {
    float4 position [[position]];
    float3 fragPosition;
    float3 color;
    float3 normal;
    float4 fragPosLightSpace;
};

struct InstanceSceneVertexUniform {
    uint scene_size;
    float scalefactor;
    packed_float3 scene_offset;
    
    packed_float3 lower_color;
    packed_float3 land_color;
    packed_float3 rock_color; // 15 16 empty
    float4x4 PVMatrix;
    float4x4 lightSpaceMatrix;
};

struct InstanceSceneFragmentUniform {
    float near_plane;
    float far_plane;
    packed_float3 viewPos;
    ParallelLight dirLight;
};

vertex InstanceSceneVertexOut instanceSceneVertex(constant InstanceSceneVertexIn *vertices [[buffer(0)]],
                             constant float *height [[buffer(1)]],
                             constant InstanceSceneVertexUniform &uniforms [[buffer(2)]],
                             //texture2d<float>  HeightMap [[texture(0)]],
                             //sampler           sampler2D [[sampler(0)]],
                             uint vid [[vertex_id]],
                             uint iid [[instance_id]])
{
    InstanceSceneVertexIn in = vertices[vid];
    InstanceSceneVertexOut out;

    int idx = iid / uniforms.scene_size, idy = iid % uniforms.scene_size;
    float deltax = idx - uniforms.scene_size / 2.0f, deltay = idy - uniforms.scene_size / 2.0f;
    float3 pos = float3(in.position.x + deltax, 0.0f, -in.position.y + deltay + 1) * uniforms.scalefactor;

    int index = 0;
    if (vid == 0) index = (idx + 1) * (uniforms.scene_size + 1) + idy;
    else if (vid == 1 || vid == 4) index = (idx + 1) * (uniforms.scene_size + 1) + idy + 1;
    else if (vid == 2 || vid == 3) index = idx * (uniforms.scene_size + 1) + idy;
    else index = idx * (uniforms.scene_size + 1) + idy + 1;
    pos.y = height[index];

    pos.y = pos.y * 10.0f;

    out.position = uniforms.PVMatrix * float4(pos + uniforms.scene_offset, 1.0f);
    out.fragPosition = pos + uniforms.scene_offset;
    out.fragPosLightSpace = uniforms.lightSpaceMatrix * float4(out.fragPosition, 1.0);
    if(out.fragPosition.y > 1.0f) out.color = uniforms.rock_color;
    else if(out.fragPosition.y > 0.3f) out.color = uniforms.land_color;
    else out.color = uniforms.lower_color;

    // Calculate Normal
    float3 a, b, c;
    if (vid <= 2)
    {
        float ha = height[(idx + 1) * (uniforms.scene_size + 1) + idy] * 10.0f;
        float hb = height[(idx + 1) * (uniforms.scene_size + 1) + idy + 1] * 10.0f;
        float hc = height[idx * (uniforms.scene_size + 1) + idy] * 10.0f;
        a = float3(deltax + 1.0, 0.0f, deltay) * uniforms.scalefactor + float3(0, ha, 0);
        b = float3(deltax + 1.0, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, hb, 0);
        c = float3(deltax, 0.0f, deltay) * uniforms.scalefactor + float3(0, hc, 0);
    }
    else
    {
        float ha = height[idx * (uniforms.scene_size + 1) + idy] * 10.0f;
        float hb = height[(idx + 1) * (uniforms.scene_size + 1) + idy + 1] * 10.0f;
        float hc = height[idx * (uniforms.scene_size + 1) + idy + 1] * 10.0f;
        a = float3(deltax, 0.0f, deltay) * uniforms.scalefactor + float3(0, ha, 0);
        b = float3(deltax + 1.0, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, hb, 0);
        c = float3(deltax, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, hc, 0);
    }
    out.normal = normalize(cross(a - b, c - b));
    return out;

    // Wrong version
//    InstanceSceneVertexIn in = vertices[vid];
//    InstanceSceneVertexOut out;
//
//    int idx = iid / uniforms.scene_size, idy = iid % uniforms.scene_size;
//    float deltax = idx - uniforms.scene_size / 2.0f, deltay = idy - uniforms.scene_size / 2.0f;
//    float3 pos = float3(in.position.x + deltax, 0.0f, in.position.y + deltay) * uniforms.scalefactor;
//
//    int index = 0;
//    if (vid == 0) index = (idy + 1) * (uniforms.scene_size + 1) + idx + 1;
//    else if (vid == 1 || vid == 4) index = idy * (uniforms.scene_size + 1) + idx + 1;
//    else if (vid == 2 || vid == 3) index = (idy + 1) * (uniforms.scene_size + 1) + idx;
//    else index = idy * (uniforms.scene_size + 1) + idx;
//    pos.y = height[index];
//
//    pos.y = pos.y * 10.0f;
//
//    out.position = uniforms.PVMatrix * float4(pos + uniforms.scene_offset, 1.0f);
//    out.fragPosition = pos + uniforms.scene_offset;
//    out.fragPosLightSpace = uniforms.lightSpaceMatrix * float4(out.fragPosition, 1.0);
//    if(out.fragPosition.y > 1.0f) out.color = uniforms.rock_color;
//    else if(out.fragPosition.y > 0.3f) out.color = uniforms.land_color;
//    else out.color = uniforms.lower_color;
//
//    // Calculate Normal
//    float3 a, b, c;
//    if (vid <= 2)
//    {
//        float ha = height[(idy + 1) * (uniforms.scene_size + 1) + idx + 1] * 10.0f;
//        float hb = height[idy * (uniforms.scene_size + 1) + idx + 1] * 10.0f;
//        float hc = height[(idy + 1) * (uniforms.scene_size + 1) + idx] * 10.0f;
//        a = float3(deltax + 1.0, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, ha, 0);
//        b = float3(deltax + 1.0, 0.0f, deltay + 0.0) * uniforms.scalefactor + float3(0, hb, 0);
//        c = float3(deltax + 0.0, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, hc, 0);
//    }
//    else
//    {
//        float ha = height[(idy + 1) * (uniforms.scene_size + 1) + idx] * 10.0f;
//        float hb = height[idy * (uniforms.scene_size + 1) + idx + 1] * 10.0f;
//        float hc = height[idy * (uniforms.scene_size + 1) + idx] * 10.0f;
//        a = float3(deltax + 0.0, 0.0f, deltay + 1.0) * uniforms.scalefactor + float3(0, ha, 0);
//        b = float3(deltax + 1.0, 0.0f, deltay + 0.0) * uniforms.scalefactor + float3(0, hb, 0);
//        c = float3(deltax + 0.0, 0.0f, deltay + 0.0) * uniforms.scalefactor + float3(0, hc, 0);
//    }
//    out.normal = -normalize(cross(a - b, c - b));
//    return out;
}

fragment float4 instanceSceneFragment(InstanceSceneVertexOut vert [[stage_in]],
                              constant InstanceSceneFragmentUniform &uniforms [[buffer(0)]],
                              texture2d<float>  shadowmap [[texture(0)]],
                              sampler           sampler2D [[sampler(0)]])
{
    float3 projCoords = vert.fragPosLightSpace.xyz / vert.fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float visibility;
    if(projCoords.z > 1.0)
        visibility = 1.0;
    else {
        float bias = 0.00051f;
        float currentDepth = projCoords.z;
        float closestDepth = shadowmap.sample(sampler2D, float2(projCoords.x,1 - projCoords.y)).r;
        float shadow = clamp(exp(-20.0 * (currentDepth - closestDepth - bias)), 0.0, 1.0);
        visibility = shadow;
    }
    
    // float3 viewDir = normalize(uniforms.viewPos - vert.fragPosition);
    float3 lightDir = normalize(-uniforms.dirLight.direction);
    // diffuse shading
    float diff = max(dot(vert.normal, lightDir), 0.0);
    // specular shading
    // combine results
    float3 ambient = uniforms.dirLight.ambient;
    float3 diffuse = uniforms.dirLight.diffuse * diff;
    
    float3 factory = ambient + diffuse * visibility;
    float3 aFragColor = vert.color * factory;
    
    return float4(aFragColor, 1.0);
}
