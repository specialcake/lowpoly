//
//  Scene.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import Foundation
import Metal
import MetalKit
import simd

struct InstanceSceneVertexUniform {
    var scene_size: UInt32
    var scalefactor: Float
    var scene_offset: float3
    
    var lower_color: float3
    var land_color: float3
    var rock_color: float3
    var PVMatrix: float4x4
    //var lightSpaceMatrix: float4x4
}

class Scene {
public
    let device: MTLDevice
    var debugFlag: Bool = false
    var chunk = [[Chunk]]()
    var cur_Chunk: Chunk!
    var cur_Submesh: Submesh!
    var tmp_Chunk = [Chunk]()
    
    var offset: float3!
    var chunk_offset = [[float3]](repeating: [float3](repeating: float3(0,0,0), count: CHUNK_SIZE), count: CHUNK_SIZE)
    var mesh_offset = [[float3]](repeating: [float3](repeating: float3(0,0,0), count: MESH_SIZE + 1), count: MESH_SIZE + 1)
    
    var generator: Noise!
    
    // PipelineState
    var mapPipelineState: MTLRenderPipelineState! = nil
    var mapInstancePipelineState: MTLRenderPipelineState! = nil
    var waterPipelineState: MTLRenderPipelineState! = nil
    var shadowPipelineState: MTLRenderPipelineState! = nil
    
    // Texture
    var textureLoader: MTKTextureLoader! = nil
    var HeightMap: MTLTexture! = nil
    var NormalMap0: MTLTexture! = nil
    var NormalMap1: MTLTexture! = nil
    var pNormalMap: MTLTexture! = nil
    
    // Buffer
    var vertexBuffer: MTLBuffer! = nil
    var instanceVertexBuffer: MTLBuffer! = nil
    var vertexUniformBuffer: MTLBuffer! = nil
    var fragmentUniformBuffer: MTLBuffer! = nil
    
    var vertices: [Float] = []
    
    init(device: MTLDevice, initpos: float3, shader: MTLRenderPipelineState, textureLoader: MTKTextureLoader) {
        self.device = device
        offset = initpos
        mapPipelineState = shader
        self.textureLoader = textureLoader
        
        for _ in 0 ..< 3 {
            let c = Chunk(x: 0, z: 0)
            c.parent = self
            tmp_Chunk.append(c)
        }
        
        for i in 0 ..< CHUNK_SIZE {
            var row = [Chunk]()
            for j in 0 ..< CHUNK_SIZE {
                let c = Chunk(x: i, z: j)
                c.parent = self
                row.append(c)
            }
            chunk.append(row)
        }
        
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< CHUNK_SIZE {
                updateNeighbor(i, j)
            }
        }
        
        cur_Chunk = chunk[CHUNK_RADIUS][CHUNK_RADIUS]
        cur_Submesh = cur_Chunk.submesh[MESH_RADIUS][MESH_RADIUS]
        initBuffer()
        
        generator = Noise(size: NOISE_SIZE)
        
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< CHUNK_SIZE {
                chunk_offset[i][j] = float3(Float(i - CHUNK_RADIUS) * CHUNK_LENGTH, 0.0, Float(j - CHUNK_RADIUS) * CHUNK_LENGTH)
            }
        }
        
        for i in 0 ... MESH_SIZE {
            for j in 0 ... MESH_SIZE {
                mesh_offset[i][j] = float3((Float(i - MESH_RADIUS) - 0.5) * MESH_LENGTH, 0.0, (Float(j - MESH_RADIUS) - 0.5) * MESH_LENGTH)
            }
        }
    }
    
    func initBuffer() {
        
        vertices = [
            // Pos
            1.0, 1.0,
            1.0, 0.0,
            0.0, 1.0,
            
            0.0, 1.0,
            1.0, 0.0,
            0.0, 0.0]
        
        vertexBuffer = device.makeBuffer(bytes: vertices, length: MemoryLayout<Float>.size * vertices.count, options:[])
        
        let instance_vertices: [Float] = [
            1.0, 1.0, -1.0,
            1.0, 0.0, -1.0,
            0.0, 1.0, -1.0,
            
            0.0, 1.0, 1.0,
            1.0, 0.0, 1.0,
            0.0, 0.0, 1.0]
        
        instanceVertexBuffer = device.makeBuffer(bytes: instance_vertices, length: MemoryLayout<Float>.size * instance_vertices.count, options:[])
        
    }
    
    func draw(commandQueue: MTLCommandQueue, drawable: CAMetalDrawable, viewMatrix: float4x4, projectionMatrix: inout float4x4, clearColor: MTLClearColor?) {
        
        //HeightMap = Generate_HeightMap()
        
        let bleen = MTLClearColor(red: 0.5, green: 0.5, blue: 0.5, alpha: 1)
        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = drawable.texture
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].clearColor = bleen
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        
        let commandBuffer = commandQueue.makeCommandBuffer()!
        let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
        
        let sizeOfUniformsBuffer = MemoryLayout<UInt32>.size + MemoryLayout<Float>.size * 31
        let uniformsBuffer = device.makeBuffer(length: sizeOfUniformsBuffer, options: [])!
        let bufferPointer = uniformsBuffer.contents()
        
        var offset = 0
        var scene_size: UInt32 = UInt32(MESH_SIZE * CHUNK_SIZE)
        var scalefactor: Float = MESH_LENGTH
        var scene_offset: float3 = self.offset
        
        var lower_color: float3 = LOWER_COLOR
        var land_color: float3 = LAND_COLOR
        var rock_color: float3 = ROCK_COLOR
        var PVMatrix: float4x4 = projectionMatrix * viewMatrix
        //var lightSpaceMatrix: float4x4
        memcpy(bufferPointer + offset, &scene_size, MemoryLayout<UInt32>.size)
        offset += MemoryLayout<UInt32>.size
        memcpy(bufferPointer + offset, &scalefactor, MemoryLayout<Float>.size)
        offset += MemoryLayout<Float>.size
        memcpy(bufferPointer + offset, &scene_offset, MemoryLayout<Float>.size * 3)
        offset += MemoryLayout<Float>.size * 3
        memcpy(bufferPointer + offset, &lower_color, MemoryLayout<Float>.size * 3)
        offset += MemoryLayout<Float>.size * 3
        memcpy(bufferPointer + offset, &land_color, MemoryLayout<Float>.size * 3)
        offset += MemoryLayout<Float>.size * 3
        memcpy(bufferPointer + offset, &rock_color, MemoryLayout<Float>.size * 3)
        offset += MemoryLayout<Float>.size * 5
        memcpy(bufferPointer + offset, &PVMatrix, MemoryLayout<Float>.size * float4x4.numberOfElements)
        offset += MemoryLayout<Float>.size * float4x4.numberOfElements
        
        renderEncoder.setRenderPipelineState(mapInstancePipelineState)
        renderEncoder.setVertexBuffer(instanceVertexBuffer, offset: 0, index: 0)
        renderEncoder.setVertexBuffer(uniformsBuffer, offset: 0, index: 1)
        
        // 面剔除
        renderEncoder.setFrontFacing(.counterClockwise)
        renderEncoder.setCullMode(.back)
        
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6, instanceCount: MESH_SIZE * MESH_SIZE * CHUNK_SIZE * CHUNK_SIZE)
        
        renderEncoder.endEncoding()
        
        commandBuffer.present(drawable)
        commandBuffer.commit()
    }
    
    func generate_scene() {
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< CHUNK_SIZE {
                chunk[i][j].generate_height()
            }
        }
        
        /* water and normal
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< CHUNK_SIZE {
                chunk[i][j].updateInfo()
            }
         }
        */
    }

    func updateChunks() {
        debugFlag = false
        var dir: Direction = .ORIGIN_POS
        var Chunkx: Int = 0
        var Chunkz: Int = 0
        var Meshx: Int = 0
        var Meshz: Int = 0
        GetLocationbyCamera(&Chunkx, &Chunkz, &Meshx, &Meshz)
        
        if (Chunkx == cur_Chunk.pos_x && Chunkz == cur_Chunk.pos_z) {
            cur_Submesh = cur_Chunk.submesh[Meshx][Meshz]
            return
        }
        debugFlag = true
        var toUpdate = [[Chunk]]()
        if (Chunkx > cur_Chunk.pos_x) {
            offset += float3(CHUNK_LENGTH, 0.0, 0.0)
            var row = [Chunk]()
            for i in 0 ..< CHUNK_SIZE {
                var tmpChunk = chunk[0][i]
                for j in 1 ..< CHUNK_SIZE {
                    chunk[j - 1][i] = chunk[j][i]
                    chunk[j - 1][i].pos_x = j - 1
                }
                chunk[CHUNK_SIZE - 1][i] = tmpChunk
                tmpChunk.recycle(x: CHUNK_SIZE - 1, z: i)
                row.append(tmpChunk)
            }
            toUpdate.append(row)
            dir = .MOVE_XPOS
        }
        else if (Chunkx < cur_Chunk.pos_x) {
            offset -= float3(CHUNK_LENGTH, 0.0, 0.0)
            var row = [Chunk]()
            for i in 0 ..< CHUNK_SIZE {
                var tmpChunk = chunk[CHUNK_SIZE - 1][i]
                for j in (1 ..< CHUNK_SIZE).reversed() {
                    chunk[j][i] = chunk[j - 1][i]
                    chunk[j][i].pos_x = j
                }
                chunk[0][i] = tmpChunk
                tmpChunk.recycle(x: 0, z: i)
                row.append(tmpChunk)
            }
            toUpdate.append(row)
            dir = .MOVE_XNEG
        }
        if (Chunkz > cur_Chunk.pos_z) {
            offset += float3(0.0, 0.0, CHUNK_LENGTH)
            var row = [Chunk]()
            for i in 0 ..< CHUNK_SIZE {
                var tmpChunk = chunk[i][0]
                for j in 1 ..< CHUNK_SIZE {
                    chunk[i][j - 1] = chunk[i][j]
                    chunk[i][j - 1].pos_z = j - 1
                }
                chunk[i][CHUNK_SIZE - 1] = tmpChunk
                tmpChunk.recycle(x: i, z: CHUNK_SIZE - 1)
                row.append(tmpChunk)
            }
            toUpdate.append(row)
            dir = .MOVE_ZPOS
        }
        else if (Chunkz < cur_Chunk.pos_z) {
            offset -= float3(0.0, 0.0, CHUNK_LENGTH)
            var row = [Chunk]()
            for i in 0 ..< CHUNK_SIZE {
                var tmpChunk = chunk[i][CHUNK_SIZE - 1]
                for j in (1 ..< CHUNK_SIZE).reversed() {
                    chunk[i][j] = chunk[i][j - 1]
                    chunk[i][j].pos_z = j
                }
                chunk[i][0] = tmpChunk
                tmpChunk.recycle(x: i, z: 0)
                row.append(tmpChunk)
            }
            toUpdate.append(row)
            dir = .MOVE_ZNEG
        }
        for i in 0 ..< CHUNK_SIZE {
            updateNeighbor(0, i)
            updateNeighbor(1, i)
            updateNeighbor(i, 0)
            updateNeighbor(i, 1)
            updateNeighbor(CHUNK_SIZE - 1, i)
            updateNeighbor(CHUNK_SIZE - 2, i)
            updateNeighbor(i, CHUNK_SIZE - 1)
            updateNeighbor(i, CHUNK_SIZE - 2)
        }
        
        for row in toUpdate {
            for c in row {
                c.updateInfo()
            }
        }
        
        cur_Chunk = chunk[CHUNK_RADIUS][CHUNK_RADIUS]
        cur_Submesh = cur_Chunk.submesh[MESH_RADIUS][MESH_RADIUS]
        //    UpdateChunk(dir)
    }
    
    func updateNeighbor(_ x: Int, _ y: Int) {
        if (x > 0) {
            chunk[x][y].xNeg = chunk[x - 1][y]
        }
        else {
            chunk[x][y].xNeg = nil
        }
        if (y > 0) {
            chunk[x][y].zNeg = chunk[x][y - 1]
        }
        else {
            chunk[x][y].zNeg = nil
        }
        if (x < CHUNK_SIZE - 1) {
            chunk[x][y].xPos = chunk[x + 1][y]
        }
        else {
            chunk[x][y].xPos = nil
        }
        if (y < CHUNK_SIZE - 1) {
            chunk[x][y].zPos = chunk[x][y + 1]
        }
        else {
            chunk[x][y].zPos = nil
        }
    }
    
    func GetLocationbyCamera(_ cx: inout Int , _ cz: inout Int, _ mx: inout Int, _ mz: inout Int) {
        var position = ResourceManager.camera.position -
        chunk[CHUNK_RADIUS][CHUNK_RADIUS].submesh[MESH_RADIUS][MESH_RADIUS].get_Position() -
        float3(MESH_LENGTH / 2.0, 0.0, MESH_LENGTH / 2.0)
        position = float3(0.0) - chunk[CHUNK_RADIUS][CHUNK_RADIUS].submesh[MESH_RADIUS][MESH_RADIUS].get_Position() - float3(MESH_LENGTH / 2.0, 0.0, MESH_LENGTH / 2.0)
        //    printf("camera position = ")
        //    Tools::PrintVec3(ResourceManager::camera.Position)
        //    printf("center position = ")
        //    Tools::PrintVec3(chunk[CHUNK_RADIUS][CHUNK_RADIUS].submesh[MESH_RADIUS][MESH_RADIUS].get_Position() + float3(MESH_LENGTH / 2.0, 0.0, MESH_LENGTH / 2.0))
        
        if (position.x > 0) {
            cx = Int(position.x / CHUNK_LENGTH + 0.5) + CHUNK_RADIUS
        }
        else {
            cx = Int(position.x / CHUNK_LENGTH - 0.5) + CHUNK_RADIUS
        }
        if (position.z > 0) {
            cz = Int(position.z / CHUNK_LENGTH + 0.5) + CHUNK_RADIUS
        }
        else {
            cz = Int(position.z / CHUNK_LENGTH - 0.5) + CHUNK_RADIUS
        }
        position.x -= Float(cx - CHUNK_RADIUS) * CHUNK_LENGTH
        position.z -= Float(cz - CHUNK_RADIUS) * CHUNK_LENGTH
        if (position.x > 0) {
            mx = Int(position.x / MESH_LENGTH + 0.5) + MESH_RADIUS
        }
        else {
            mx = Int(position.x / MESH_LENGTH - 0.5) + MESH_RADIUS
        }
        if (position.z > 0) {
            mz = Int(position.z / MESH_LENGTH + 0.5) + MESH_RADIUS
        }
        else {
            mz = Int(position.z / MESH_LENGTH - 0.5) + MESH_RADIUS
        }
    }
    
    func Generate_HeightMap() -> MTLTexture {
        var p: Int = 0
        var limit = CHUNK_SIZE * CHUNK_SIZE * MESH_SIZE * MESH_SIZE + CHUNK_SIZE * MESH_SIZE * 2 + 1
        var len = CHUNK_SIZE * MESH_SIZE + 1
        var data = [[Float]]()
        for i in 0 ..< CHUNK_SIZE {
            for j in 0 ..< MESH_SIZE {
                for k in 0 ..< CHUNK_SIZE {
                    data.append(chunk[i][k].height[j])
                }
            }
        }
        
        for i in 0 ..< CHUNK_SIZE {
            data.append(chunk[CHUNK_SIZE - 1][i].height[MESH_SIZE])
        }
        
        for i in 0 ..< data.count {
            for j in 0 ..< data[i].count {
                data[i][j] *= 0.1
            }
        }
        
        let textureData = NSData(bytes: &data, length: len) as! Data
        let texture = try! textureLoader.newTexture(data: textureData, options: nil)
        return texture
    }
    
//    Texture2D Generate_NormalMap(int th)
//    Texture2D Generate_pNormalMap()
//    void Generate_ShadowMap(const glm::mat4& lightSpaceMatrix, const glm::mat4& view)
}
