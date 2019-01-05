//
//  ResourceManager.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/1.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import MetalKit

class ResourceManager {
    
    static var device: MTLDevice!
    static var commandQueue: MTLCommandQueue!
    
    // renderPipelineState
    static var scenePipelineState: MTLRenderPipelineState!
    static var instanceScenePipelineState: MTLRenderPipelineState!
    static var waterPipelineState: MTLRenderPipelineState!
    static var skyboxPipelineState: MTLRenderPipelineState!
    
    // computePipelineState
    static var skymapPipelineState: MTLComputePipelineState!
    static var threadgroupSize: MTLSize!
    static var threadgroupCount: MTLSize!
    
    static var depthBufferDescriptor: MTLTextureDescriptor!
    static var depthTexture: MTLTexture!
    
    static var textureDescriptor: MTLTextureDescriptor!
    
    static var textureLoader: MTKTextureLoader! = nil
    
    static var camera: CameraController!
    static var projectionMatrix: float4x4!
}
