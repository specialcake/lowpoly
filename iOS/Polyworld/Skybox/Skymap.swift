//
//  Skymap.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/3.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import MetalKit
import simd

class Skymap {
public
    var skymap = [MTLTexture]()
    
    var vertexBuffer: MTLBuffer! = nil
    var vertexUniformBuffer: MTLBuffer! = nil
    var fragmentUniformBuffer: MTLBuffer! = nil
    
    init() {
        
        for _ in 0 ..< 6 {
            let texture = ResourceManager.device.makeTexture(descriptor: ResourceManager.textureDescriptor)!
            skymap.append(texture)
        }
        
        let vertices: [Float] = [
            // Pos
            1.0, 1.0, -1.0,
            1.0, -1.0, -1.0,
            -1.0, 1.0, -1.0,
            
            -1.0, 1.0, -1.0,
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0
       ]
        
        vertexBuffer = ResourceManager.device.makeBuffer(bytes: vertices, length: MemoryLayout<Float>.size * vertices.count, options:[])
    }
    
    /*
    func load() {
        var texture = MetalTexture.init(resourceName: "Front", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: false)
        skymap[0] = texture.texture
        
        texture = MetalTexture.init(resourceName: "Back", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: false)
        skymap[1] = texture.texture
        
        texture = MetalTexture.init(resourceName: "Right", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: false)
        skymap[2] = texture.texture
        
        texture = MetalTexture.init(resourceName: "Left", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: false)
        skymap[3] = texture.texture
        
        texture = MetalTexture.init(resourceName: "Top", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: false)
        skymap[4] = texture.texture
        
        texture = MetalTexture.init(resourceName: "Bottom", ext: "png", mipmaped: false)
        texture.loadTexture(device: ResourceManager.device, commandQ: ResourceManager.commandQueue, flip: true)
        skymap[5] = texture.texture
    } */
    
    func draw(sunPos: float3) {
        
        let commandBuffer = ResourceManager.commandQueue.makeCommandBuffer()!
        let computerEncoder = commandBuffer.makeComputeCommandEncoder()!
        
        computerEncoder.setComputePipelineState(ResourceManager.skymapPipelineState)
        computerEncoder.setTexture(skymap[0], index: 0)
        computerEncoder.setTexture(skymap[1], index: 1)
        computerEncoder.setTexture(skymap[2], index: 2)
        computerEncoder.setTexture(skymap[3], index: 3)
        computerEncoder.setTexture(skymap[4], index: 4)
        computerEncoder.setTexture(skymap[5], index: 5)
        computerEncoder.dispatchThreadgroups(ResourceManager.threadgroupCount, threadsPerThreadgroup: ResourceManager.threadgroupSize)
        
        computerEncoder.endEncoding()
        commandBuffer.commit()
        
    }
}
