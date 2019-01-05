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
    static var commandBuffer: MTLCommandBuffer!
    
    // PipelineState
    static var scenePipelineState: MTLRenderPipelineState!
    static var instanceScenePipelineState: MTLRenderPipelineState!
    static var waterPipelineState: MTLRenderPipelineState!
    static var skymapPipelineState: MTLRenderPipelineState!
    static var skyboxPipelineState: MTLRenderPipelineState!
    
    static var depthBufferDescriptor: MTLTextureDescriptor!
    static var depthTexture: MTLTexture!
    
    static var textureDescriptor: MTLTextureDescriptor!
    
    static var textureLoader: MTKTextureLoader! = nil
    
    static var camera: CameraController!
    
    static var skybox: Skybox!
}
