
import Foundation
import MetalKit
import simd

class Sun: NSObject {
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
    
    init(device: MTLDevice, modelMatrix: float4x4, forResourse name: String, withExtension ext: String) {
        self.device = device
        self.modelMatrix = modelMatrix
        commandQueue = device.makeCommandQueue()!
        vertexDescriptor = Sun.buildVertexDescriptor(device: device)
        renderPipeline = Sun.buildPipeline(device: device, vertexDescriptor: vertexDescriptor)
        depthStencilState = Sun.buildDepthStencilState(device: device)
        textureLoader = MTKTextureLoader(device: device)
        (defaultTexture, defaultNormalMap) = Sun.buildDefaultTextures(device: device)
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
        
        let vertexFunction = library.makeFunction(name: "sunVertex")
        let fragmentFunction = library.makeFunction(name: "sunFragment")
        
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
        renderPassDescriptor.depthAttachment.loadAction = .clear
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
}
