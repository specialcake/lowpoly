//
//  Noise.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation
import simd

class Noise {
public
    init(size: Int) {
        self.size = size
    
        P[0] = 2; P[1] = 3; P[2] = 15; P[3] = 26; P[4] = 11;
        P[5] = 14; P[6] = 4; P[7] = 19; P[8] = 1; P[9] = 22;
        P[10] = 27; P[11] = 23; P[12] = 6; P[13] = 12; P[14] = 18;
        P[15] = 17; P[16] = 5; P[17] = 10; P[18] = 9; P[19] = 24;
        P[20] = 7; P[21] = 20; P[22] = 13; P[23] = 25; P[24] = 0;
        P[25] = 28; P[26] = 21; P[27] = 16; P[28] = 8;
        G[0] = float2(0.684770, -0.728759); G[1] = float2(0.031835, -0.999493); G[2] = float2(-0.999705, 0.024282); G[3] = float2(-0.165815, 0.986157);
        G[4] = float2(-0.576616, 0.817016); G[5] = float2(0.989431, 0.145007); G[6] = float2(0.994057, -0.108861); G[7] = float2(-0.684243, 0.729254);
        G[8] = float2(0.499701, 0.866198); G[9] = float2(0.644026, -0.765004); G[10] = float2(-0.615800, 0.787902); G[11] = float2(0.926926, 0.375244);
        G[12] = float2(0.904772, 0.425897); G[13] = float2(0.528370, 0.849014); G[14] = float2(-0.996860, -0.079180); G[15] = float2(-0.999941, -0.010838);
        G[16] = float2(0.088838, -0.996046); G[17] = float2(0.716457, 0.697631); G[18] = float2(-0.991936, -0.126737); G[19] = float2(0.829506, 0.558497);
        G[20] = float2(-0.226003, 0.974127); G[21] = float2(0.980895, 0.194538); G[22] = float2(0.946064, 0.323980); G[23] = float2(-0.619802, -0.784758);
        G[24] = float2(-0.842756, 0.538296); G[25] = float2(-0.950774, 0.309886); G[26] = float2(-0.925944, -0.377660); G[27] = float2(-0.562101, 0.827068);
        G[28] = float2(0.957367, -0.288874);
    }
    
    func Generate(loc: float3) -> Float {
        var p = float2(loc.x, loc.z)
        p *= 0.1
        //    Tools::PrintVec2(p);
        var ret = default_noise(p: p)
        //    printf("height => %.3lf\n", ret);
        ret = ret * 0.5 + 0.25
        ret *= 3.0
        if(ret < 0.0) {
            ret = 0.0
        }
        ret = pow(ret, 3.76)
        return ret
    }
    
private
    var size: Int
    var G = [float2](repeating: float2(0,0), count: 30)
    var P = [Int](repeating: 0, count: 30)
    
    func processer(p: float2) -> float2 {
        
        //    Tools::PrintVec2(p);
        //    printf("====>");
        var i: Int
        var pp = p
        
        if (pp.x < 0) {
            i = 1
            while (pp.x + Float(i * size) < 0) {
                i <<= 1
            }
            while (i > 0) {
                if(pp.x + Float(i * size) < Float(size)) {
                    pp.x += Float(i * size);
                }
                i >>= 1
            }
        }
        else if (pp.x >= Float(size)){
            i = 1
            while (pp.x - Float(i * size) > 0) {
                i <<= 1
            }
            while (i > 0) {
                if(pp.x - Float(i * size) > 0) {
                    pp.x -= Float(i * size);
                }
                i >>= 1
            }
        }
        
        if (pp.y < 0) {
            i = 1
            while (pp.y + Float(i * size) < 0) {
                i <<= 1
            }
            while (i > 0) {
                if(pp.y + Float(i * size) < Float(size)) {
                    pp.y += Float(i * size);
                }
                i >>= 1
            }
        }
        else if (pp.y >= Float(size)){
            i = 1
            while (pp.y - Float(i * size) > 0) {
                i <<= 1
            }
            while (i > 0) {
                if(pp.y - Float(i * size) > 0) {
                    pp.y -= Float(i * size);
                }
                i >>= 1
            }
        }
        return pp;
    }
    
    func default_noise(p: float2) -> Float {
        //    static bool debug_flag = true;
        var ret: Float = 0.0
        //    printf("%f\n", ret);
        var A: Float = 1.0
        var pp = p
        
        for _ in 0 ..< 5 {
            pp = processer(p: pp);
            ret += A * perlin_noise(p: pp);
            //        if(debug_flag == true){
            //            Tools::PrintVec2(p);
            //            printf("==> %f\n", ret);
            //        }
            pp = 2.0 * pp
            A = 0.5 * A
        }
        //    debug_flag = false;
        //    ret += 1.0000f * perlin_noise(p); p = 2.0f * p;
        //    printf("%f\n", ret);
        //    ret += 0.5000f * perlin_noise(p); p = 2.0f * p;
        //    printf("%f\n", ret);
        //    ret += 0.2500f * perlin_noise(p); p = 2.0f * p;
        //    printf("%f\n", ret);
        //    ret += 0.1250f * perlin_noise(p); p = 2.0f * p;
        //    printf("%f\n", ret);
        //    ret += 0.0625f * perlin_noise(p); p = 2.0f * p;
        //    printf("%f\n", ret);
        return ret;
    }
    
    func perlin_noise(p: float2) -> Float {
        let pi = floor(p)
        let pf = p - pi
        var w = pf * pf * pf * (6.0 * pf * pf - 15.0 * pf + float2(10.0,10.0))
        
        //    GLfloat vala = glm::dot(get_vector(pi + float2(0.0f, 0.0f)), pf - float2(0.0f, 0.0f));
        //    GLfloat valb = glm::dot(get_vector(pi + float2(1.0f, 0.0f)), pf - float2(1.0f, 0.0f));
        //    GLfloat valc = glm::dot(get_vector(pi + float2(0.0f, 1.0f)), pf - float2(0.0f, 1.0f));
        //    GLfloat vald = glm::dot(get_vector(pi + float2(1.0f, 1.0f)), pf - float2(1.0f, 1.0f));
        //
        //    if(debug_flag == true){
        //        printf("======\n");
        //        Tools::PrintVec4(float4(vala, valb, valc, vald));
        //        printf("======\n");
        //    }
        //    debug_flag = false;
        //
        //    return glm::mix(glm::mix(vala, valb, w.x), glm::mix(valc, vald, w.x), w.y);
        
        return mix(
            mix(dot(get_vector(p: pi + float2(0.0, 0.0)), pf - float2(0.0, 0.0)),dot(get_vector(p: pi + float2(1.0, 0.0)), pf - float2(1.0, 0.0)), w.x),
            mix(dot(get_vector(p: pi + float2(0.0, 1.0)), pf - float2(0.0, 1.0)),dot(get_vector(p: pi + float2(1.0, 1.0)), pf - float2(1.0, 1.0)), w.x),
            w.y)
    }
    
    func get_vector(p: float2) -> float2 {
        let x = Int(p.x), y = Int(p.y)
        return G[(x + P[y % size]) % size]
    }
    
    func mix(_ x: Float, _ y: Float, _ a: Float) -> Float {
        return x * (1 - a) + y * a
    }
}
