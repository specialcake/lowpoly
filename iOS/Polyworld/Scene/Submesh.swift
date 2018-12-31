//
//  Submesh.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation
import simd

enum MeshType {
    case LAND, WATER
}

class Submesh {
public
    var pos_x, pos_z: Int
    var meshType: MeshType
    var occupied: Bool
    var parent: Chunk!
    var xPos, xNeg, zPos, zNeg: Submesh?
    
    init(x: Int, z: Int) {
        pos_x = x
        pos_z = z
        
        meshType = .LAND
        occupied = false
        
        xPos = nil
        xNeg = nil
        zPos = nil
        zNeg = nil
    }
    
    func draw_map() {
        
    }
    
    func draw_water() {
        
    }
    
    func get_Position() -> float3 {
        let top_scene = parent.parent!
        var ret = top_scene.offset!
        ret += top_scene.chunk_offset[parent.pos_x][parent.pos_z]
        ret += top_scene.mesh_offset[pos_x][pos_z]
        return ret
    }
    
    func recycle(_ x: Int, _ z: Int) {
        pos_x = x
        pos_z = z
        
        occupied = false
        
        xPos = nil
        xNeg = nil
        zPos = nil
        zNeg = nil
    }
}
