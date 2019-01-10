//
//  Shadowmap.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/8.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import Metal
import simd

class Shadowmap {
    var depthMap, bluredShadow: MTLTexture!
    var near, far, left, right, top, bottom: Float!
    
    init() {
        depthMap = ResourceManager.device.makeTexture(descriptor: ResourceManager.depthBufferDescriptor)!
    }
    
    func UpdateFrustum(scene: Scene){
        let lightviewmat = lookAt(PARLIGHT_POSITION + scene.offset, PARLIGHT_POSITION + scene.offset + PARLIGHT_DIR, float3(0.0, 1.0, 0.0))
        near = 200000
        left = 200000
        bottom = 200000
        far = -200000
        right = -200000
        top = -200000
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< CHUNK_SIZE {
                for k in 0 ... MESH_SIZE {
                    for h in 0 ... MESH_SIZE {
                        var pos = scene.offset + scene.chunk_offset[i][j] + scene.mesh_offset[k][h]
                        pos.y = scene.chunk[i][j].height[k][h]
                        let tmpPos = pos
                        pos = (lightviewmat * float4(tmpPos.x, tmpPos.y, tmpPos.z, 1.0)).xyz
                        left   = min(left,   pos.x)
                        right  = max(right,  pos.x)
                        bottom = min(bottom, pos.y)
                        top    = max(top,    pos.y)
                        near   = min(near,  -pos.z)
                        far    = max(far ,  -pos.z)
                    }
                }
            }
        }
        near -= 0.2
        left -= 0.2
        bottom -= 0.2
        far += 0.2
        right += 0.2
        top += 0.2
    }
    
    func GetlightSpaceMatrix(scene: Scene) -> float4x4 {
        let lightProjection = float4x4(orthoProjectionLeft: left, right: right, bottom:     bottom, top: top, near: near, far: far)
        let lightviewmat = lookAt(PARLIGHT_POSITION + scene.offset, PARLIGHT_POSITION + scene.offset + PARLIGHT_DIR, float3(0.0, 1.0, 0.0))
        let lightSpaceMatrix = lightProjection * lightviewmat
        return lightSpaceMatrix
    }
    
    func BeginMakeMap() {
        
    }
    
    func EndMakeMap() {
        
    }
}
