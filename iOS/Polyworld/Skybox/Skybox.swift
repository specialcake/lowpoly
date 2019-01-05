//
//  Skybox.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/3.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import MetalKit

class Skybox {
    
    var vertexBuffer: MTLBuffer! = nil
    var vertexUniformBuffer: MTLBuffer! = nil
    
    var samplerState: MTLSamplerState!
    
    init() {
        let skyboxVertices: [Float] = [
            -1.0,  1.0, -1.0, 4.0,
            -1.0, -1.0, -1.0, 4.0,
            1.0, -1.0, -1.0, 4.0,
            1.0, -1.0, -1.0, 4.0,
            1.0,  1.0, -1.0, 4.0,
            -1.0,  1.0, -1.0, 4.0,
            
            -1.0, -1.0,  1.0, 1.0,
            -1.0, -1.0, -1.0, 1.0,
            -1.0,  1.0, -1.0, 1.0,
            -1.0,  1.0, -1.0, 1.0,
            -1.0,  1.0,  1.0, 1.0,
            -1.0, -1.0,  1.0, 1.0,
            
            1.0, -1.0, -1.0, 0.0,
            1.0, -1.0,  1.0, 0.0,
            1.0,  1.0,  1.0, 0.0,
            1.0,  1.0,  1.0, 0.0,
            1.0,  1.0, -1.0, 0.0,
            1.0, -1.0, -1.0, 0.0,
            
            -1.0, -1.0,  1.0, 5.0,
            -1.0,  1.0,  1.0, 5.0,
            1.0,  1.0,  1.0, 5.0,
            1.0,  1.0,  1.0, 5.0,
            1.0, -1.0,  1.0, 5.0,
            -1.0, -1.0,  1.0, 5.0,
            
            -1.0,  1.0, -1.0, 2.0,
            1.0,  1.0, -1.0, 2.0,
            1.0,  1.0,  1.0, 2.0,
            1.0,  1.0,  1.0, 2.0,
            -1.0,  1.0,  1.0, 2.0,
            -1.0,  1.0, -1.0, 2.0,
            
            -1.0, -1.0, -1.0, 3.0,
            -1.0, -1.0,  1.0, 3.0,
            1.0, -1.0, -1.0, 3.0,
            1.0, -1.0, -1.0, 3.0,
            -1.0, -1.0,  1.0, 3.0,
            1.0, -1.0,  1.0, 3.0
        ]
        
        vertexBuffer = ResourceManager.device.makeBuffer(bytes: skyboxVertices, length: MemoryLayout<Float>.size * skyboxVertices.count, options:[])
        
        samplerState = defaultSampler(device: ResourceManager.device)
    }
    
    func draw(drawable: CAMetalDrawable, skymap: Skymap, viewMatrix: float4x4, projectionMatrix: float4x4) {
        
        let white = MTLClearColor(red: 1, green: 1, blue: 1, alpha: 1)
        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = drawable.texture
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].clearColor = white
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        
        let commandBuffer = ResourceManager.commandQueue.makeCommandBuffer()!
        let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
        
        let sizeOfUniformsBuffer = MemoryLayout<Float>.size * 16
        vertexUniformBuffer = ResourceManager.device.makeBuffer(length: sizeOfUniformsBuffer, options: [])!
        let bufferPointer = vertexUniformBuffer.contents()
        var tempViewMatrix = viewMatrix;
        tempViewMatrix[0][3] = 0;
        tempViewMatrix[1][3] = 0;
        tempViewMatrix[2][3] = 0;
        tempViewMatrix[3][0] = 0;
        tempViewMatrix[3][1] = 0;
        tempViewMatrix[3][2] = 0;
        tempViewMatrix[3][3] = 1;
        var PVMatrix: float4x4 = projectionMatrix * tempViewMatrix
        memcpy(bufferPointer, &PVMatrix, MemoryLayout<Float>.size * float4x4.numberOfElements)
        
        renderEncoder.setRenderPipelineState(ResourceManager.skyboxPipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder.setFragmentTexture(skymap.skymap[0], index: 0)
        renderEncoder.setFragmentTexture(skymap.skymap[1], index: 1)
        renderEncoder.setFragmentTexture(skymap.skymap[2], index: 2)
        renderEncoder.setFragmentTexture(skymap.skymap[3], index: 3)
        renderEncoder.setFragmentTexture(skymap.skymap[4], index: 4)
        renderEncoder.setFragmentTexture(skymap.skymap[5], index: 5)
        renderEncoder.setFragmentSamplerState(samplerState, index: 0)
        
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 36)
        renderEncoder.endEncoding()
        commandBuffer.commit()
    }
    
    func defaultSampler(device: MTLDevice) -> MTLSamplerState {
        let sampler = MTLSamplerDescriptor()
        sampler.minFilter             = MTLSamplerMinMagFilter.nearest
        sampler.magFilter             = MTLSamplerMinMagFilter.nearest
        sampler.mipFilter             = MTLSamplerMipFilter.nearest
        sampler.maxAnisotropy         = 1
        sampler.sAddressMode          = MTLSamplerAddressMode.clampToEdge
        sampler.tAddressMode          = MTLSamplerAddressMode.clampToEdge
        sampler.rAddressMode          = MTLSamplerAddressMode.clampToEdge
        sampler.normalizedCoordinates = true
        sampler.lodMinClamp           = 0
        sampler.lodMaxClamp           = FLT_MAX
        return device.makeSamplerState(descriptor: sampler)!
    }
}
