//
//  Config.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation
import simd

let eps = 0.0000001

// Screen
let NEAR_PLANE: Float = 0.1
let FAR_PLANE: Float = 150.0

// Scene
let SCENE_LENGTH: Float = 53.0

let CHUNK_SIZE = 3 // Scene含有的Chunk个数
let CHUNK_RADIUS = 1
let CHUNK_LENGTH = SCENE_LENGTH / Float(CHUNK_SIZE)

let MESH_SIZE = 21 // Chunk含有的Mesh个数
let MESH_RADIUS = 10
let MESH_LENGTH = CHUNK_LENGTH / Float(MESH_SIZE)

let WATER_COLOR = float3(0.106, 0.863, 1.0)
let LOWER_COLOR = float3(0.75686, 1.0, 0.91373)
let LAND_COLOR = float3(0.48627, 0.97647, 0.65882)
let ROCK_COLOR = float3(0.522, 0.522, 0.522)

let NOISE_SIZE = 29

enum Direction {
    case ORIGIN_POS,
    MOVE_XPOS,
    MOVE_XNEG,
    MOVE_ZPOS,
    MOVE_ZNEG
}

// Water
let NUMOFWATER = 4
let SEA_LEVEL: Float = 0.30
let CONSTQ: Float = 0.5

// Light
let PARLIGHT_DIR = float3(0.0, -13.0, 27.0)
let PARLIGHT_POSITION = float3(0.0, 13.0, -27.0)
let PARLIGHT_AMBIENT = float3(0.3)
let PARLIGHT_DIFFUSE = float3(0.9)
let PARLIGHT_SPECULAR = float3(0.5)
