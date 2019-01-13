//
//  SkymapShader.metal
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/3.
//  Copyright © 2019年 Sxy. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#define PI 3.1415926535
#define iSteps 16
#define jSteps 8

float2 rsi(float3 r0, float3 rd, float sr) {
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    if (d < 0.0) return float2(1e5,-1e5);
    return float2(
                  (-b - sqrt(d))/(2.0*a),
                  (-b + sqrt(d))/(2.0*a)
                  );
}

float3 atmosphere(float3 r, float3 r0, float3 pSun, float iSun, float rPlanet, float rAtmos, float3 kRlh, float kMie, float shRlh, float shMie, float g) {
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);
    
    // Calculate the step size of the primary ray.
    float2 p = rsi(r0, r, rAtmos);
    if (p.x > p.y) return float3(0,0,0);
    p.y = min(p.y, rsi(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);
    
    // Initialize the primary ray time.
    float iTime = 0.0;
    
    // Initialize accumulators for Rayleigh and Mie scattering.
    float3 totalRlh = float3(0,0,0);
    float3 totalMie = float3(0,0,0);
    
    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;
    
    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));
    
    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {
        
        // Calculate the primary ray sample position.
        float3 iPos = r0 + r * (iTime + iStepSize * 0.5);
        
        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;
        
        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;
        
        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;
        
        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);
        
        // Initialize the secondary ray time.
        float jTime = 0.0;
        
        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;
        
        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {
            
            // Calculate the secondary ray sample position.
            float3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);
            
            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;
            
            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;
            
            // Increment the secondary ray time.
            jTime += jStepSize;
        }
        
        // Calculate attenuation.
        float3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));
        
        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;
        
        // Increment the primary ray time.
        iTime += iStepSize;
        
    }
    
    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

float4 GetColor(float3 vPosition, float3 uSunPos){
    float3 color = atmosphere(
        normalize(vPosition),           // normalized ray direction
        float3(0,6372e3,0),             // ray origin
        uSunPos,                        // position of the sun
        32.0,                           // intensity of the sun
        6371e3,                         // radius of the planet in meters
        6471e3,                         // radius of the atmosphere in meters
        float3(5.5e-6, 13.0e-6, 22.4e-6), // Rayleigh scattering coefficient
        21e-6,                          // Mie scattering coefficient
        8e3,                            // Rayleigh scale height
        1.2e3,                          // Mie scale height
        0.758                           // Mie preferred scattering direction
    );
    
    color = 1.0 - exp(-1.0 * color);
    
    return float4(color, 1.0f);
}

kernel void skymapCompute(texture2d<float, access::write> front  [[texture(0)]],
                          texture2d<float, access::write> back   [[texture(1)]],
                          texture2d<float, access::write> right  [[texture(2)]],
                          texture2d<float, access::write> left   [[texture(3)]],
                          texture2d<float, access::write> top    [[texture(4)]],
                          texture2d<float, access::write> bottom [[texture(5)]],
                          uint2 gid [[thread_position_in_grid]])
{
    int width = front.get_width();
    int height = front.get_height();
    float3 sunPos = float3(0.0, 0.5, -1.0);
    float2 uv = float2(gid) / float2(width, height);
    float2 pos = uv * 2 - 1;
    float3 fragPosition = float3(pos.x, pos.y, -1.0);
    float4 color = GetColor(fragPosition, sunPos);
    front.write(color, gid);

    fragPosition = float3(pos.x, pos.y, 1.0);
    color = GetColor(fragPosition, sunPos);
    back.write(color, gid);
    
    fragPosition = float3(1.0, pos.y, pos.x);
    color = GetColor(fragPosition, sunPos);
    right.write(color, gid);
    
    fragPosition = float3(-1.0, pos.y, pos.x);
    color = GetColor(fragPosition, sunPos);
    left.write(color, gid);
    
    fragPosition = float3(pos.x, 1.0, pos.y);
    color = GetColor(fragPosition, sunPos);
    top.write(color, gid);
    
    fragPosition = float3(pos.x, -1.0, pos.y);
    color = GetColor(fragPosition, sunPos);
    bottom.write(color, gid);
}
