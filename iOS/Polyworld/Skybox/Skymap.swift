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
    }
    
    func draw(drawable: CAMetalDrawable, sunPos: float3) {
        var uSunPos = sunPos
        var sizeOfUniformsBuffer = MemoryLayout<Float>.size * 3
        fragmentUniformBuffer = ResourceManager.device.makeBuffer(length: sizeOfUniformsBuffer, options: [])!
        var bufferPointer = fragmentUniformBuffer.contents()
        memcpy(bufferPointer, &uSunPos, MemoryLayout<Float>.size * 3)

        let bleen = MTLClearColor(red: 0, green: 1, blue: 1, alpha: 1)
        let renderPassDescriptor0 = MTLRenderPassDescriptor()
        renderPassDescriptor0.colorAttachments[0].texture = skymap[0]
        renderPassDescriptor0.colorAttachments[0].loadAction = .clear
        renderPassDescriptor0.colorAttachments[0].clearColor = bleen
        renderPassDescriptor0.colorAttachments[0].storeAction = .store
        let renderEncoder0 = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor0)!
        
        sizeOfUniformsBuffer = MemoryLayout<Float>.size
        vertexUniformBuffer = ResourceManager.device.makeBuffer(length: sizeOfUniformsBuffer, options: [])!
        bufferPointer = vertexUniformBuffer.contents()
        
        var type: Float = 0.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        
        renderEncoder0.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder0.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder0.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder0.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder0.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder0.endEncoding()
        
        let renderPassDescriptor1 = MTLRenderPassDescriptor()
        renderPassDescriptor1.colorAttachments[0].texture = skymap[1]
        renderPassDescriptor1.colorAttachments[0].loadAction = .clear
        renderPassDescriptor1.colorAttachments[0].clearColor = bleen
        renderPassDescriptor1.colorAttachments[0].storeAction = .store
        let renderEncoder1 = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor1)!
        type = 1.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        renderEncoder1.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder1.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder1.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder1.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder1.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder1.endEncoding()

        let renderPassDescriptor2 = MTLRenderPassDescriptor()
        renderPassDescriptor2.colorAttachments[0].texture = skymap[2]
        renderPassDescriptor2.colorAttachments[0].loadAction = .clear
        renderPassDescriptor2.colorAttachments[0].clearColor = bleen
        renderPassDescriptor2.colorAttachments[0].storeAction = .store
        var renderEncoder = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor2)!
        type = 2.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        renderEncoder.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder.endEncoding()

        let renderPassDescriptor3 = MTLRenderPassDescriptor()
        renderPassDescriptor3.colorAttachments[0].texture = skymap[3]
        renderPassDescriptor3.colorAttachments[0].loadAction = .clear
        renderPassDescriptor3.colorAttachments[0].clearColor = bleen
        renderPassDescriptor3.colorAttachments[0].storeAction = .store
        renderEncoder = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor3)!
        type = 3.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        renderEncoder.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder.endEncoding()

        let renderPassDescriptor4 = MTLRenderPassDescriptor()
        renderPassDescriptor4.colorAttachments[0].texture = skymap[4]
        renderPassDescriptor4.colorAttachments[0].loadAction = .clear
        renderPassDescriptor4.colorAttachments[0].clearColor = bleen
        renderPassDescriptor4.colorAttachments[0].storeAction = .store
        renderEncoder = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor4)!
        type = 4.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        renderEncoder.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder.endEncoding()
        
        let renderPassDescriptor5 = MTLRenderPassDescriptor()
        renderPassDescriptor5.colorAttachments[0].texture = skymap[5]
        renderPassDescriptor5.colorAttachments[0].loadAction = .clear
        renderPassDescriptor5.colorAttachments[0].clearColor = bleen
        renderPassDescriptor5.colorAttachments[0].storeAction = .store
        renderEncoder = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor5)!
        type = 5.0
        memcpy(bufferPointer, &type, MemoryLayout<Float>.size)
        renderEncoder.setRenderPipelineState(ResourceManager.skymapPipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(vertexUniformBuffer, offset: 0, index: 1)
        renderEncoder.setFragmentBuffer(fragmentUniformBuffer, offset: 0, index: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
        renderEncoder.endEncoding()
    }
    
}
