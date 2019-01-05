//
//  ParallelLight.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/1.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import simd

struct ParallelLight {
    
    var direction: float3
    var ambient: float3
    var diffuse: float3
    var specular: float3
    
    static func size() -> Int {
        return MemoryLayout<Float>.size * 12
    }
    
    func raw() -> [Float] {
        let raw = [direction.x, direction.y, direction.z, ambient.x, ambient.y, ambient.z, diffuse.x, diffuse.y, diffuse.z, diffuse.x, diffuse.y, diffuse.z]
        return raw
    }
}
