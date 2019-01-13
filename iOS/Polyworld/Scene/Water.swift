//
//  Water.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/2.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import simd

struct Wave{
    //Amplitude, Q, Speed, Length, phi, omega
    //Direction
    var A, Q, S, L, phi, w: Float
    var D: float2
};

class Water {
public
    var wave = [Wave]()
    
    init() {
        wave.append(Wave(A: 0.05, Q: 0, S: 0.6, L: 2.1, phi: 0, w: 0, D: float2(-0.468,-0.956)))
        wave.append(Wave(A: 0.02, Q: 0, S: 0.4, L: 3.5, phi: 0, w: 0, D: float2(-0.111,0.379)))
        wave.append(Wave(A: 0.07, Q: 0, S: 0.5, L: 4.2, phi: 0, w: 0, D: float2(0.53,-0.239)))
        wave.append(Wave(A: 0.03, Q: 0, S: 0.7, L: 5.0, phi: 0, w: 0, D: float2(0.497,0.78)))
        
        for i in 0 ..< wave.count {
            wave[i].w = Float(sqrt(9.8 * 2 * .pi / wave[i].L))
            wave[i].phi = wave[i].S * wave[i].w
            wave[i].Q = CONSTQ / (wave[i].w * wave[i].A * Float(NUMOFWATER))
        }
    }
    
    func raw() -> [Float] {
        var raw = [Float]()
        for i in 0 ..< wave.count {
            raw += [wave[i].A, wave[i].Q, wave[i].phi, wave[i].w, wave[i].D.x, wave[i].D.y]
        }
        return raw
    }
    
}
