//
//  Polyball.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/11.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation
import MetalKit
import simd

struct Collision{
    var exist: Bool
    var Point: float3
    var Normal: float3
}

struct Coord{
    var Front: float3
    var Right: float3
    var Up: float3
}

class Polyball: NSObject {
    let device: MTLDevice
    let commandQueue: MTLCommandQueue
    let renderPipeline: MTLRenderPipelineState
    let depthStencilState: MTLDepthStencilState
    let vertexDescriptor: MDLVertexDescriptor
    
    let textureLoader: MTKTextureLoader
    let defaultTexture: MTLTexture
    let defaultNormalMap: MTLTexture
    //let irradianceCubeMap: MTLTexture
    
    var nodes = [Node]()
    
    var modelMatrix = matrix_identity_float4x4
    var viewMatrix = matrix_identity_float4x4
    var projectionMatrix = matrix_identity_float4x4
    var cameraWorldPosition = float3(0, 0, 0)
    var cameraFront =  float3(0, 0, -1)
    var lightWorldDirection = float3(0, 1, 0)
    var lightWorldPosition = float3(0, 5, 0)
    var time: Float = 0
    
    var speed: float3 = float3(0.0, 0.0, 0.0)
    var speedLimit: float3 = float3(0.0, 0.0, 0.0)
    var Acceleration: float3 = float3(2.0, 0.0, 2.0)
    var Resistance: float3 = float3(1.0, 3.0, 1.0)
    var Position: float3 = float3(0.0, 0.01, 4.0)
    var Radius: Float = 0.5 * 0.3
    var Mov: Coord = Coord(Front: float3(1.0, 0.0, 0.0), Right: float3(0.0, 0.0, 1.0), Up: float3(0.0, 1.0, 0.0))
    var Cam: Coord = Coord(Front: ResourceManager.camera.front, Right: ResourceManager.camera.right, Up: ResourceManager.camera.up)
    var collision: Collision = Collision(exist: false, Point: float3(), Normal: float3())
    
    var wspeed: Float!
    var rotate_axis: float3!
    var rotate_state: simd_quatf = simd_quatf(real: 1.0, imag: float3())
    
    var Maycol = [[float3]](repeating: [float3](repeating: float3(0,0,0), count: 5), count: 5)
    
    var dcx,dcy,dmx,dmy: Int!
    
    init(device: MTLDevice, modelMatrix: float4x4, forResourse name: String, withExtension ext: String) {
        self.device = device
        self.modelMatrix = modelMatrix
        commandQueue = device.makeCommandQueue()!
        vertexDescriptor = Polyball.buildVertexDescriptor(device: device)
        renderPipeline = Polyball.buildPipeline(device: device, vertexDescriptor: vertexDescriptor)
        depthStencilState = Polyball.buildDepthStencilState(device: device)
        textureLoader = MTKTextureLoader(device: device)
        (defaultTexture, defaultNormalMap) = Polyball.buildDefaultTextures(device: device)
        //irradianceCubeMap = Renderer.buildEnvironmentTexture("garage_pmrem.ktx", device: device)
        super.init()
        
        guard let modelURL = Bundle.main.url(forResource: name, withExtension: ext) else {
            fatalError("Could not find model file in app bundle")
        }
        buildScene(url: modelURL, device: device, vertexDescriptor: vertexDescriptor)
    }
    
    static func buildVertexDescriptor(device: MTLDevice) -> MDLVertexDescriptor {
        let vertexDescriptor = MDLVertexDescriptor()
        vertexDescriptor.attributes[0] = MDLVertexAttribute(name: MDLVertexAttributePosition,
                                                            format: .float3,
                                                            offset: 0,
                                                            bufferIndex: VertexBufferIndex.attributes.rawValue)
        vertexDescriptor.attributes[1] = MDLVertexAttribute(name: MDLVertexAttributeNormal,
                                                            format: .float3,
                                                            offset: MemoryLayout<Float>.size * 3,
                                                            bufferIndex: VertexBufferIndex.attributes.rawValue)
        vertexDescriptor.attributes[2] = MDLVertexAttribute(name: MDLVertexAttributeTangent,
                                                            format: .float3,
                                                            offset: MemoryLayout<Float>.size * 6,
                                                            bufferIndex: VertexBufferIndex.attributes.rawValue)
        vertexDescriptor.attributes[3] = MDLVertexAttribute(name: MDLVertexAttributeTextureCoordinate,
                                                            format: .float2,
                                                            offset: MemoryLayout<Float>.size * 9,
                                                            bufferIndex: VertexBufferIndex.attributes.rawValue)
        vertexDescriptor.layouts[VertexBufferIndex.attributes.rawValue] = MDLVertexBufferLayout(stride: MemoryLayout<Float>.size * 11)
        return vertexDescriptor
    }
    
    static func buildPipeline(device: MTLDevice, vertexDescriptor: MDLVertexDescriptor) -> MTLRenderPipelineState {
        guard let library = device.makeDefaultLibrary() else {
            fatalError("Could not load default library from main bundle")
        }
        
        let vertexFunction = library.makeFunction(name: "polyballVertex")
        let fragmentFunction = library.makeFunction(name: "polyballFragment")
        
        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunction
        pipelineDescriptor.fragmentFunction = fragmentFunction
        
        pipelineDescriptor.colorAttachments[0].pixelFormat = .bgra8Unorm
        pipelineDescriptor.depthAttachmentPixelFormat = .depth32Float
        
        let mtlVertexDescriptor = MTKMetalVertexDescriptorFromModelIO(vertexDescriptor)
        pipelineDescriptor.vertexDescriptor = mtlVertexDescriptor
        
        do {
            return try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
        } catch {
            fatalError("Could not create render pipeline state object: \(error)")
        }
    }
    
    static func buildDepthStencilState(device: MTLDevice) -> MTLDepthStencilState {
        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.depthCompareFunction = .less
        depthStencilDescriptor.isDepthWriteEnabled = true
        return device.makeDepthStencilState(descriptor: depthStencilDescriptor)!
    }
    
    static func buildDefaultTextures(device: MTLDevice) -> (MTLTexture, MTLTexture) {
        let bounds = MTLRegionMake2D(0, 0, 1, 1)
        let descriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .rgba8Unorm,
                                                                  width: bounds.size.width,
                                                                  height: bounds.size.height,
                                                                  mipmapped: false)
        descriptor.usage = .shaderRead
        let defaultTexture = device.makeTexture(descriptor: descriptor)!
        let defaultColor: [UInt8] = [ 0, 0, 0, 255 ]
        defaultTexture.replace(region: bounds, mipmapLevel: 0, withBytes: defaultColor, bytesPerRow: 4)
        let defaultNormalMap = device.makeTexture(descriptor: descriptor)!
        let defaultNormal: [UInt8] = [ 127, 127, 255, 255 ]
        defaultNormalMap.replace(region: bounds, mipmapLevel: 0, withBytes: defaultNormal, bytesPerRow: 4)
        return (defaultTexture, defaultNormalMap)
    }
    
    static func buildEnvironmentTexture(_ name: String, device:MTLDevice) -> MTLTexture {
        let textureLoader = MTKTextureLoader(device: device)
        let options: [MTKTextureLoader.Option : Any] = [:]
        do {
            let textureURL = Bundle.main.url(forResource: name, withExtension: nil)!
            let texture = try textureLoader.newTexture(URL: textureURL, options: options)
            return texture
        } catch {
            fatalError("Could not load irradiance map from asset catalog: \(error)")
        }
    }
    
    func buildScene(url: URL, device: MTLDevice, vertexDescriptor: MDLVertexDescriptor) {
        let bufferAllocator = MTKMeshBufferAllocator(device: device)
        let asset = MDLAsset(url: url, vertexDescriptor: nil, bufferAllocator: bufferAllocator)
        
        asset.loadTextures()
        
        for sourceMesh in asset.childObjects(of: MDLMesh.self) as! [MDLMesh] {
            sourceMesh.addOrthTanBasis(forTextureCoordinateAttributeNamed: MDLVertexAttributeTextureCoordinate,
                                       normalAttributeNamed: MDLVertexAttributeNormal,
                                       tangentAttributeNamed: MDLVertexAttributeTangent)
            sourceMesh.vertexDescriptor = vertexDescriptor
        }
        
        guard let (sourceMeshes, meshes) = try? MTKMesh.newMeshes(asset: asset, device: device) else {
            fatalError("Could not convert ModelIO meshes to MetalKit meshes")
        }
        
        for (sourceMesh, mesh) in zip(sourceMeshes, meshes) {
            var materials = [Material]()
            for sourceSubmesh in sourceMesh.submeshes as! [MDLSubmesh] {
                let material = Material(material: sourceSubmesh.material, textureLoader: textureLoader)
                materials.append(material)
            }
            let node = Node(mesh: mesh, materials: materials)
            nodes.append(node)
        }
    }
    
    func updateScene(drawable: CAMetalDrawable) {
        //time += 1 / Float(view.preferredFramesPerSecond)
        modelMatrix = float4x4(translationBy: Position) * float4x4(rotate_state)
        projectionMatrix = ResourceManager.projectionMatrix
        viewMatrix = ResourceManager.camera.viewMatrix
        cameraWorldPosition = ResourceManager.camera.position
        lightWorldPosition = PARLIGHT_POSITION
        lightWorldDirection = normalize(PARLIGHT_DIR)
    }
    
    func bindTextures(_ material: Material, _ commandEncoder: MTLRenderCommandEncoder) {
        commandEncoder.setFragmentTexture(material.baseColor ?? defaultTexture, index: TextureIndex.baseColor.rawValue)
        commandEncoder.setFragmentTexture(material.metallic ?? defaultTexture, index: TextureIndex.metallic.rawValue)
        commandEncoder.setFragmentTexture(material.roughness ?? defaultTexture, index: TextureIndex.roughness.rawValue)
        commandEncoder.setFragmentTexture(material.normal ?? defaultNormalMap, index: TextureIndex.normal.rawValue)
        commandEncoder.setFragmentTexture(material.emissive ?? defaultTexture, index: TextureIndex.emissive.rawValue)
    }
    
    func draw(drawable: CAMetalDrawable) {
        updateScene(drawable: drawable)
        
        let white = MTLClearColor(red: 1, green: 1, blue: 1, alpha: 1)
        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = drawable.texture
        renderPassDescriptor.colorAttachments[0].loadAction = .load
        renderPassDescriptor.colorAttachments[0].clearColor = white
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        
        renderPassDescriptor.depthAttachment.texture = ResourceManager.depthTexture
        renderPassDescriptor.depthAttachment.loadAction = .load
        renderPassDescriptor.depthAttachment.storeAction = .store
        
        let commandEncoder = ResourceManager.commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
        commandEncoder.setRenderPipelineState(renderPipeline)
        commandEncoder.setDepthStencilState(depthStencilState)
        
        for node in nodes {
            draw(node, in: commandEncoder)
        }
        
        commandEncoder.endEncoding()
    }
    
    func draw(_ node: Node, in commandEncoder: MTLRenderCommandEncoder) {
        let mesh = node.mesh
        
        var uniforms = Uniforms(modelMatrix: modelMatrix,
                                viewMatrix: viewMatrix,
                                projectionMatrix: projectionMatrix,
                                cameraPosition: cameraWorldPosition,
                                lightDirection: lightWorldDirection,
                                lightPosition: lightWorldPosition)
        commandEncoder.setVertexBytes(&uniforms, length: MemoryLayout<Uniforms>.size, index: VertexBufferIndex.uniforms.rawValue)
        commandEncoder.setFragmentBytes(&uniforms, length: MemoryLayout<Uniforms>.size, index: FragmentBufferIndex.uniforms.rawValue)
        
        for (bufferIndex, vertexBuffer) in mesh.vertexBuffers.enumerated() {
            commandEncoder.setVertexBuffer(vertexBuffer.buffer, offset: vertexBuffer.offset, index: bufferIndex)
        }
        
        for (submeshIndex, submesh) in mesh.submeshes.enumerated() {
            let material = node.materials[submeshIndex]
            bindTextures(material, commandEncoder)
            
            let indexBuffer = submesh.indexBuffer
            commandEncoder.drawIndexedPrimitives(type: submesh.primitiveType,
                                                 indexCount: submesh.indexCount,
                                                 indexType: submesh.indexType,
                                                 indexBuffer: indexBuffer.buffer,
                                                 indexBufferOffset: indexBuffer.offset)
        }
    }
    
    func GenCameraPosition() -> float3 {
        let distance: Float = 3.0
        let cosvalue: Float = cos(.pi / 12.0)
        let sinvalue: Float = sin(.pi / 12.0)
        var ret = Position
        ret += -distance * cosvalue * Cam.Front
        ret += distance * sinvalue * Cam.Up
        return ret
    }
    
    func CollisionCheck(scene: Scene) {
        collision = Collision(exist: false, Point: float3(), Normal: float3())
        var cx: Int = 0, cy: Int = 0, mx: Int = 0, my: Int = 0
        GetChunkMeshID(scene: scene, cx: &cx, cz: &cy, mx: &mx, mz: &my)
        dcx = cx
        dcy = cy
        dmx = mx
        dmy = my
        for i in -1 ... 2 {
            for j in -1 ... 2 {
                var meshx = mx + i
                var meshy = my + j
                var chunkx, chunky: Int
                if (meshx < 0) {
                    chunkx = cx - 1
                    meshx += MESH_SIZE
                } else if (meshx >= MESH_SIZE) {
                    chunkx = cx + 1
                    meshx -= MESH_SIZE
                } else {
                    chunkx = cx
                }
                
                if (meshy < 0) {
                    chunky = cy - 1
                    meshy += MESH_SIZE
                }
                else if (meshy >= MESH_SIZE) {
                    chunky = cy + 1
                    meshy -= MESH_SIZE
                }
                else {
                    chunky = cy
                }
                
                Maycol[i + 1][j + 1] = scene.chunk_offset[chunkx][chunky] + scene.mesh_offset[meshx][meshy]
                Maycol[i + 1][j + 1].y = scene.chunk[chunkx][chunky].height[meshx][meshy]
                if (Maycol[i + 1][j + 1].y < 0.1) {
                    Maycol[i + 1][j + 1].y = -0.5
                }
            }
        }

        var delta_pos = float3(0.0)
        for i in 0 ..< 3 {
            for j in 0 ..< 3 {
                var a, b, c: float3
                var dist: Float
                a = Maycol[i][j]
                b = Maycol[i + 1][j]
                c = Maycol[i + 1][j + 1]
                dist = distance(Position, a, b, c)
                if (dist <= Radius) {
                    collision.exist = true
                    collision.Normal += normalize(cross(c - a, b - a))
                    delta_pos += (Radius - dist) * collision.Normal
                }
                b = c
                c = Maycol[i][j + 1]
                dist = distance(Position, a, b, c)
                if (dist <= Radius) {
                    collision.exist = true
                    collision.Normal += normalize(cross(c - a, b - a))
                    delta_pos += (Radius - dist) * collision.Normal
                }
            }
        }
        Position += delta_pos
        if (collision.exist) {
            collision.Normal = normalize(collision.Normal)
        }
    }
    
    func GetChunkMeshID(scene: Scene, cx: inout Int, cz: inout Int, mx: inout Int, mz: inout Int) {
        var position = Position - scene.chunk[CHUNK_RADIUS][CHUNK_RADIUS].submesh[MESH_RADIUS][MESH_RADIUS].get_Position() - float3(MESH_LENGTH / 2.0, 0.0, MESH_LENGTH / 2.0)
        
        if (position.x > 0) {
            cx = Int(position.x / CHUNK_LENGTH + 0.5) + CHUNK_RADIUS
        } else {
            cx = Int(position.x / CHUNK_LENGTH - 0.5) + CHUNK_RADIUS
        }
        if (position.z > 0) {
            cz = Int(position.z / CHUNK_LENGTH + 0.5) + CHUNK_RADIUS
        } else {
            cz = Int(position.z / CHUNK_LENGTH - 0.5) + CHUNK_RADIUS
        }
        position.x -= Float(cx - CHUNK_RADIUS) * CHUNK_LENGTH
        position.z -= Float(cz - CHUNK_RADIUS) * CHUNK_LENGTH
        if (position.x > 0) {
            mx = Int(position.x / MESH_LENGTH + 0.5) + MESH_RADIUS
        } else {
            mx = Int(position.x / MESH_LENGTH - 0.5) + MESH_RADIUS
        }
        if (position.z > 0) {
            mz = Int(position.z / MESH_LENGTH + 0.5) + MESH_RADIUS
        } else {
            mz = Int(position.z / MESH_LENGTH - 0.5) + MESH_RADIUS
        }
    }
    
    func UpdatePosition(deltaTime: Float, scene: Scene) {
        Position += speed * deltaTime
        CollisionCheck(scene: scene)
        
        let angle = wspeed * deltaTime / 2.0
        let rot = simd_quatf(real: cos(angle),
                             imag: float3(sin(angle) * rotate_axis.x,
                                          sin(angle) * rotate_axis.y,
                                          sin(angle) * rotate_axis.z)
                            )
        rotate_state = rot * rotate_state
    }
    
    func UpdateSpeed(deltaTime: Float) {
        if (collision.exist) {
            let velocity = -dot(collision.Normal, speed)
            speed += velocity * collision.Normal
            if (velocity > 2.0) {
                speed += 0.1 * velocity * collision.Normal
            }
        }
        UpdateMovVec()
        
        var delta_v = float3(0.0)
        var noKeyboard = true
        if (collision.exist){
            if (ResourceManager.keys[BallMovement.forward.rawValue]) {
                let cosvalue = dot(Cam.Front, Mov.Up)
                delta_v += (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime
                noKeyboard = false
            }
            if (ResourceManager.keys[BallMovement.backward.rawValue]) {
                let cosvalue = dot(Cam.Front, Mov.Up)
                delta_v -= (Cam.Front - cosvalue * Mov.Up) * Acceleration.x * deltaTime
                noKeyboard = false
            }
            if (ResourceManager.keys[BallMovement.right.rawValue]) {
                let cosvalue = dot(Cam.Right, Mov.Up)
                delta_v += (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime
                noKeyboard = false
            }
            if (ResourceManager.keys[BallMovement.left.rawValue]) {
                let cosvalue = dot(Cam.Right, Mov.Up)
                delta_v -= (Cam.Right - cosvalue * Mov.Up) * Acceleration.z * deltaTime
                noKeyboard = false
            }
            if (ResourceManager.keys[BallMovement.jump.rawValue]) {
                delta_v += float3(0.0, 3.0, 0.0)
            }
            //Sliding Friction
            let movXZ = speed
            let movSpeed = length(movXZ)
            if (Double(movSpeed) > eps) {
                if (Resistance.x * deltaTime > movSpeed && noKeyboard) {
                    delta_v -= normalize(movXZ) * movSpeed
                } else {
                delta_v -= normalize(movXZ) * Resistance.x * deltaTime
                }
            }
        }
        //Gravity
        delta_v.y -= Resistance.y * deltaTime
        speed += delta_v
        
        UpdateMovVec()
        
        wspeed = length(speed) / Radius
        rotate_axis = -Mov.Right
    }
    
    func UpdateMovVec() {
        let speed = self.speed
        
        if(Double(abs(speed.x)) < eps && Double(abs(speed.x)) < eps) {
            Mov.Front = float3(1.0, 0.0, 0.0);
            Mov.Right = float3(0.0, 0.0, 1.0);
            Mov.Up    = float3(0.0, 1.0, 0.0);
        }
        else {
            Mov.Front = normalize(speed);
            Mov.Right = normalize(cross(Mov.Front, float3(0.0, 1.0, 0.0)));
            Mov.Up    = normalize(cross(Mov.Right, Mov.Front));
        }
    }
    
}
