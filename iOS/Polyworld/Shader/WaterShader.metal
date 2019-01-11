//
//  WaterShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/2.
//  Copyright © 2019年 Sxy. All rights reserved.
//

#include <metal_stdlib>
#define wave_number 4
using namespace metal;

struct Wave{
    float A, Q, phi, w;
    packed_float2 D;
};

struct ParallelLight{
    packed_float3 direction;
    packed_float3 ambient;
    packed_float3 diffuse;
    packed_float3 specular;
};

struct WaterVertexOut {
    float4 position [[position]];
    float3 fragPosition;
    float3 normal;
    float4 fragPosLightSpace;
};

struct WaterVertexUniform {
    uint scene_size;
    float scalefactor;
    packed_float3 scene_offset;
    
    float water_height;
    float timer; // 8 empty
    float4x4 PVMatrix;
    float4x4 lightSpaceMatrix;
    
    Wave wave[wave_number];
};


struct WaterFragmentUniform {
    packed_float3 viewPos;
    packed_float3 water_color;
    ParallelLight dirLight;
};

vertex WaterVertexOut waterVertex(constant float2 *vertices [[buffer(0)]],
                                  constant WaterVertexUniform &uniforms [[buffer(1)]],
                                  //texture2d<float>  HeightMap [[texture(0)]],
                                  //sampler           sampler2D [[sampler(0)]],
                                  uint vid [[vertex_id]],
                                  uint iid [[instance_id]])
{
    float2 in = vertices[vid];
    WaterVertexOut out;
    
    int idx = iid / uniforms.scene_size, idy = iid % uniforms.scene_size;
    float deltax = idx - uniforms.scene_size / 2.0f, deltay = idy - uniforms.scene_size / 2.0f;
    float3 pos = float3(in.x + deltax, 0.0f, in.y + deltay) * uniforms.scalefactor;
    
    pos.y = uniforms.water_height;
    pos += uniforms.scene_offset;
    
    float3 opos = pos;
    for(int i = 0; i < wave_number; i++) {
        float Qs = 1.0 / (wave_number * uniforms.wave[i].w * uniforms.wave[i].A);
        pos.x += Qs * uniforms.wave[i].A * uniforms.wave[i].D.x * cos(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
        pos.z += Qs * uniforms.wave[i].A * uniforms.wave[i].D.y * cos(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
        pos.y += uniforms.wave[i].A * sin(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
    }
    
    out.position = uniforms.PVMatrix * float4(pos, 1.0f);
    out.fragPosition = pos;
    out.fragPosLightSpace = uniforms.lightSpaceMatrix * float4(out.fragPosition, 1.0);
    
    // Calculate Normal
    float3 a,b,c;
    float3 v[3];
    float2 offset[3];
    
    if (vid == 0)
    {
        offset[0] = float2(0,0);
        offset[1] = float2(0,-1);
        offset[2] = float2(-1,0);
    }
    else if (vid == 1)
    {
        offset[0] = float2(0,1);
        offset[1] = float2(0,0);
        offset[2] = float2(-1,1);
    }
    else if (vid == 2)
    {
        offset[0] = float2(1,0);
        offset[1] = float2(1,-1);
        offset[2] = float2(0,0);
    }
    else if (vid == 3)
    {
        offset[0] = float2(0,0);
        offset[1] = float2(1,-1);
        offset[2] = float2(0,-1);
    }
    else if (vid == 4)
    {
        offset[0] = float2(-1,1);
        offset[1] = float2(0,0);
        offset[2] = float2(-1,0);
    }
    else if (vid == 5)
    {
        offset[0] = float2(0,1);
        offset[1] = float2(1,0);
        offset[2] = float2(0,0);
    }
    v[0] = float3(in.x + deltax + offset[0].x, 0.0f, in.y + deltay + offset[0].y) * uniforms.scalefactor + float3(0, uniforms.water_height, 0);
    v[1] = float3(in.x + deltax + offset[1].x, 0.0f, in.y + deltay + offset[1].y) * uniforms.scalefactor + float3(0, uniforms.water_height, 0);
    v[2] = float3(in.x + deltax + offset[2].x, 0.0f, in.y + deltay + offset[2].y) * uniforms.scalefactor + float3(0, uniforms.water_height, 0);
    v[0] += uniforms.scene_offset;
    v[1] += uniforms.scene_offset;
    v[2] += uniforms.scene_offset;
    for (int i = 0; i < 2; i++)
    {
        float3 opos = v[i];
        for(int i = 0; i < wave_number; i++) {
            float Qs = 1.0 / (wave_number * uniforms.wave[i].w * uniforms.wave[i].A);
            v[i].x += Qs * uniforms.wave[i].A * uniforms.wave[i].D.x * cos(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
            v[i].z += Qs * uniforms.wave[i].A * uniforms.wave[i].D.y * cos(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
            v[i].y += uniforms.wave[i].A * sin(uniforms.wave[i].w * dot(float2(uniforms.wave[i].D), opos.xz) + uniforms.wave[i].phi * uniforms.timer);
        }
    }
    out.normal = -normalize(cross(v[0] - v[1], v[2] - v[1]));
    return out;
}

fragment float4 waterFragment(WaterVertexOut vert [[stage_in]],
                              constant WaterFragmentUniform &uniforms [[buffer(0)]],
                              texture2d<float>  shadowmap [[texture(0)]],
                              sampler           sampler2D [[sampler(0)]])
{
    /*
//    float3 projCoords = vert.fragPosLightSpace.xyz / vert.fragPosLightSpace.w;
//    projCoords = projCoords * 0.5 + 0.5;
//    float visibility;
//    if(projCoords.z > 1.0)
//        visibility = 1.0;
//    else {
//        float bias = 0.00051f;
//        float currentDepth = projCoords.z;
//        float closestDepth = shadowmap.sample(sampler2D, projCoords.xy).r;
//        float shadow = clamp(exp(-20.0f * (currentDepth - closestDepth - bias)), 0.0, 1.0);
//        visibility = shadow;
//    }
    
    float3 projCoords = vert.fragPosLightSpace.xyz / vert.fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float bias = 0.00051f;
    float currentDepth = projCoords.z;
    float closestDepth = shadowmap.sample(sampler2D, projCoords.xy).r;
    float shadow = clamp(exp(-20.0 * (currentDepth - closestDepth - bias)), 0.0, 1.0);
    
    float visibility;
    if (currentDepth > closestDepth) visibility = 0;
    else visibility = 1;
    
    //float visibility = shadow;
    */
    float visibility = 1;
    
    float3 viewDir = normalize(uniforms.viewPos - vert.fragPosition);
    float3 lightDir = normalize(-uniforms.dirLight.direction);
    // diffuse shading
    float diff = max(dot(vert.normal, lightDir), 0.0);
    // specular shading
    float3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(vert.normal, halfwayDir), 0.0), 8.0f);
    // combine results
    float3 ambient = uniforms.dirLight.ambient;
    float3 diffuse = uniforms.dirLight.diffuse * diff;
    float3 specular = uniforms.dirLight.specular * spec;
    float3 factory = ambient + (diffuse + specular) * visibility;
    
    float3 aFragColor = uniforms.water_color * factory;
    
    // return float4(uniforms.water_color, 0.6);
    return float4(aFragColor, 0.6);
}
