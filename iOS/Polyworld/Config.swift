//
//  Config.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation

let SCENE_LENGTH: Float = 53.0

let CHUNK_SIZE = 3 // Scene含有的Chunk个数
let CHUNK_RADIUS = 1
let CHUNK_LENGTH = SCENE_LENGTH / Float(CHUNK_SIZE)

let MESH_SIZE = 21 // Chunk含有的Mesh个数
let MESH_RADIUS = 10
let MESH_LENGTH = CHUNK_LENGTH / Float(MESH_SIZE)

let NOISE_SIZE = 29

enum Direction {
    case ORIGIN_POS,
    MOVE_XPOS,
    MOVE_XNEG,
    MOVE_ZPOS,
    MOVE_ZNEG
}
