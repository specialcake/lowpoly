//
//  GaussblurShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/13.
//  Copyright © 2019年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;
#define sigma 3.0f
#define pi 3.1415926535

struct GaussblurVertexIn {
    packed_float2 position;
    packed_float2 texCoords;
};

struct GaussblurVertexOut {
    float4 position [[position]];
    float2 texCoords;
};

struct GaussblurFragmentUniform {
    bool horizontal;
};

vertex GaussblurVertexOut gaussblurVertex(constant GaussblurVertexIn *vertices [[buffer(0)]],
                                          uint vid [[vertex_id]])
{
    GaussblurVertexIn in = vertices[vid];
    GaussblurVertexOut out;
    
    out.position = float4(in.position, 0.0, 1.0);
    out.texCoords = in.texCoords;
    return out;
}

fragment float4 gaussblurFragment(GaussblurVertexOut vert [[stage_in]],
                                  constant GaussblurFragmentUniform &uniforms [[buffer(0)]],
                                  texture2d<float>  image     [[texture(0)]],
                                  sampler           sampler2D [[sampler(0)]])
{
    vert.texCoords.y = 1.0 - vert.texCoords.y;
    float3 incrementalGaussian = float3(0.0f);
    incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
    incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;
    
    float4 avgValue = float4(0.0f);
    float coefficientSum = 0.0f;
    
    avgValue += image.sample(sampler2D, vert.texCoords) * incrementalGaussian.x;
    coefficientSum += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
    
    int width = image.get_width();
    int height = image.get_height();
    float2 tex_offset = float2(1.0 / width, 1.0 / height);
    float2 delta = float2(0.0f, 0.0f);
    if (uniforms.horizontal) delta = float2(1.0f, 0.0f);
    else delta = float2(0.0f, 1.0f);
    
    for(int i = 1; i < 3; i++){
        avgValue += image.sample(sampler2D, vert.texCoords - i * tex_offset * delta) * incrementalGaussian.x;
        avgValue += image.sample(sampler2D, vert.texCoords + i * tex_offset * delta) * incrementalGaussian.x;
        coefficientSum += 2.0 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }
    
    return avgValue / coefficientSum;
}

