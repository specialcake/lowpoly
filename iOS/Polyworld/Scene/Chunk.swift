//
//  Chunk.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation
import simd

class Chunk {
public
    var pos_x, pos_z: Int
    var submesh = [[Submesh]]()
    var height = [[Float]]()
    var normal = [[[float3]]]()
    var pnormal = [[float3]]()
    var parent: Scene!
    var xPos, xNeg, zPos, zNeg: Chunk?
    
    init(x: Int, z: Int) {
        pos_x = x
        pos_z = z
        
        xPos = nil
        xNeg = nil
        zPos = nil
        zNeg = nil
        
        for i in 0...MESH_SIZE {
            var row = [Submesh]()
            for j in 0...MESH_SIZE {
                row.append(Submesh(x: i, z: j))
                row[j].parent = self
            }
            submesh.append(row)
        }
    }
    
    func generate_height() {
        for i in 0...MESH_SIZE {
            for j in 0...MESH_SIZE {
                height[i][j] = parent.generator.Generate(loc: submesh[i][j].get_Position())
                submesh[i][j].meshType = .LAND
            }
        }
    }
    
    func generate_water() {
        
    }
    
    func generate_mountain() {
        
    }
    
    func generate_tree() {
        
    }
    
    func generate_normal() {
        
    }
    
    func calc_normal(x: Int, z: Int) -> float3 {
        return float3()
    }
    
    func draw() {
        
    }
    
    func draw_map() {
        
    }
    
    func draw_water() {
        
    }
    
    func draw_mountain() {
        
    }
    
    func draw_tree() {
        
    }
    
    func updateInfo() {
        generate_water()
        generate_normal()
    }
    
    func recycle(x: Int, z: Int) {
        xPos = nil
        xNeg = nil
        zPos = nil
        zNeg = nil
        pos_x = x
        pos_z = z
        for i in 0 ..< MESH_SIZE {
            for j in 0 ..< MESH_SIZE {
                submesh[i][j].recycle(i, j)
            }
        }
        generate_height()
    }
}
