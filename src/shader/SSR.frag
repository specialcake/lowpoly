#version 330 core

out vec4 FragColor;

in VS_OUT{
    vec2 FragPos;
    vec2 TexCoords;
    vec4 cameraRay;
} fs_in;

const float MaxRayDistance = 500.0f;
const float PixelStrideZCuttoff = 200.0f;
const float PixelWidth = 1.0 / 1200.0;
const float PixelHeight = 1.0 / 900.0;
const int PixelStride = 3;
const int Iterations = 70;
const int BinarySearchIterations = 64;

uniform float near;
uniform float far;

uniform mat4 projection;

uniform sampler2D DepthMap;
uniform sampler2D NormalMap;
uniform sampler2D ColorMap;
uniform sampler2D Reflectable;

vec2 traceSSR(vec3 rayOrigin, vec3 rayDirection, float jitter);
float linearDepth(vec2 uv);
float distance(vec2 a, vec2 b);

void main() {
    vec4 OriginColor = texture(ColorMap, fs_in.TexCoords);
    vec4 Reflectrate = texture(Reflectable, fs_in.TexCoords);

    if(Reflectrate == vec4(1.0f)){
        float curDepth = linearDepth(fs_in.TexCoords);

        vec3 vsRayOrigin = vec3(fs_in.cameraRay) * curDepth;
        vec3 vsNormal = texture(NormalMap, fs_in.TexCoords).rgb * 2.0 - 1.0;
        vec3 vsRayDirection = normalize(reflect(normalize(vsRayOrigin), normalize(vsNormal)));

        vec2 Coords = fs_in.TexCoords * vec2(1200.0, 900.0);
        float c = (Coords.x + Coords.y) * 0.25;
        float jitter = mod(c, 1.0);

        vec2 hitPixel = traceSSR(vsRayOrigin, vsRayDirection, jitter);
        FragColor = vec4(hitPixel, 0.0, 1.0);
        return ;

        if(hitPixel.x > -1.0){
            vec4 hitTexel = texture(ColorMap, hitPixel);
            FragColor = mix(OriginColor, hitTexel, 0.5);
            FragColor = vec4(hitPixel, 0.0, 1.0);
            //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else{
            FragColor = OriginColor;
        }
    }
    else{
        FragColor = texture(ColorMap, fs_in.TexCoords);
    }
}

vec2 traceSSR(vec3 rayOrigin, vec3 rayDirection, float jitter){
    float rayLength = rayOrigin.z + rayDirection.z * MaxRayDistance > -near ?
                      (-near - rayOrigin.z) / rayDirection.z : MaxRayDistance;
    vec3 rayEnd = rayOrigin + rayDirection * rayLength;

    vec4 ClipSpaceOrigin = projection * vec4(rayOrigin, 1.0);
    vec4 ClipSpaceEnd    = projection * vec4(rayEnd   , 1.0);

    float k0 = 1.0 / ClipSpaceOrigin.w, k1 = 1.0 / ClipSpaceEnd.w;
    vec3 Q0 = rayOrigin * k0, Q1 = rayEnd * k1;
    vec2 P0 = ClipSpaceOrigin.xy * k0, P1 = ClipSpaceEnd.xy * k1;

    P1 += (distance(P1, P0) < 0.0001) ? 0.01 : 0.0;
    vec2 delta = P1 - P0;

//    return normalize(delta);

    bool permute = false;
    if(abs(delta.x) < abs(delta.y)){
        permute = true;
        delta = delta.yx;
        P0 = P0.yx;
        P1 = P1.yx;
    }

    float stepDir = delta.x > 0.0 ? 1.0 : delta.x < 0.0 ? -1.0 : 0.0;
    float invdx = stepDir / delta.x;

    vec3 dQ = (Q1 - Q0) * invdx;
    float dk = (k1 - k0) * invdx;
    vec2 dP = vec2(stepDir, delta.y * invdx);

    float strideScaler = 1.0 - min( 1.0, -rayOrigin.z / PixelStrideZCuttoff);
    float pixelStride = 1.0 + strideScaler * PixelStride;

    dP *= pixelStride; dQ *= pixelStride; dk *= pixelStride;
    P0 += dP * jitter; Q0 += dQ * jitter; k0 += dk * jitter;

    float i, zA = 0.0, zB = 0.0;

    vec4 pqk = vec4(P0, Q0.z, k0);
    vec4 dPQK = vec4(dP, dQ.z, dk);

    bool interesct = false;
    vec2 hitPixel = vec2(-2.0);

    for(i = 0; i < Iterations && interesct == false; i += 1){
        pqk += dPQK;

        zA = zB;
        zB = (dPQK.z * 0.5 + pqk.z) / (dPQK.w * 0.5 + pqk.w);
        if(zB > zA) {float tmp = zB;zB = zA;zA = tmp;}

        hitPixel = permute ? pqk.yx : pqk.xy;
        hitPixel = hitPixel * vec2(PixelWidth, PixelHeight);

        float curDepth = linearDepth(hitPixel) * (-far);
        interesct = zB <= curDepth;
    }

    if(interesct == false)
        return vec2(-2.0);

    if(pixelStride > 1.0 && interesct){
        pqk -= dPQK;
        dPQK /= pixelStride;

        float originalStride = pixelStride * 0.5;
        float stride = originalStride;

        zA = pqk.z / pqk.w;
        zB = zA;

        for(float j = 0.0; j < BinarySearchIterations; j += 1.0){
            pqk += dPQK * stride;

            zA = zB;
            zB = (dPQK.z * (-0.5) + pqk.z) / (dPQK.w * (-0.5) + pqk.w);
            if(zB > zA) {float tmp = zB;zB = zA;zA = tmp;}

            hitPixel = permute ? pqk.yz : pqk.xy;
            hitPixel = hitPixel * vec2(PixelWidth, PixelHeight);

            originalStride *= 0.5;

            float curDepth = linearDepth(hitPixel) * (-far);
            bool flag = zB <= curDepth;
            if(flag) stride = -originalStride;
            else stride = originalStride;
        }
    }

    return hitPixel;
}

float distance(vec2 a, vec2 b){
    vec2 delta = b - a;
    return dot(delta, delta);
}

float linearDepth(vec2 uv){
    float depth = texture(DepthMap, uv).r;
    return (2.0 * near * far) / (far + near - depth * (far - near));
}
