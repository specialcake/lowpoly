//
//  ResourceManager.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/1.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import MetalKit

enum BallMovement: Int {
    case forward = 0
    case backward = 1
    case left = 2
    case right = 3
    case jump = 4
}

class ResourceManager {
    
    static var device: MTLDevice!
    static var commandQueue: MTLCommandQueue!
    static var commandBuffer: MTLCommandBuffer!
    
    // renderPipelineState
    static var scenePipelineState: MTLRenderPipelineState!
    static var instanceScenePipelineState: MTLRenderPipelineState!
    static var waterPipelineState: MTLRenderPipelineState!
    static var skyboxPipelineState: MTLRenderPipelineState!
    static var shadowmapPipelineState: MTLRenderPipelineState!
    static var gaussblurPipelineState: MTLRenderPipelineState!
    
    // computePipelineState
    static var skymapPipelineState: MTLComputePipelineState!
    static var threadgroupSize: MTLSize!
    static var threadgroupCount: MTLSize!
    
    static var depthBufferDescriptor: MTLTextureDescriptor!
    static var depthTexture: MTLTexture!
    static var shadowmapDepthTexture: MTLTexture!
    static var shadowmapBluredTexture: MTLTexture!
    
    // colorAttachment
    static var textureDescriptor: MTLTextureDescriptor!
    static var textureLoader: MTKTextureLoader! = nil
    
    static var camera: CameraController!
    static var projectionMatrix: float4x4!
    static var keys: [Bool] = [Bool](repeating: false, count: 5)
}
