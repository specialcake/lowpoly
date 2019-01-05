//
//  ViewController.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/12/19.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import UIKit
import Metal
import MetalKit
import QuartzCore
import simd

class ViewController: UIViewController {
    
    // Setting
    var metalLayer: CAMetalLayer! = nil
    
    // Rendering
    var timer: CADisplayLink! = nil
    var lastFrameTimestamp: CFTimeInterval = 0.0

    var trackedTouch: UITouch?
    
    var scene: Scene!
    var skymap: Skymap!
    var skybox: Skybox!
    var sun: Sun!
    
    // UI
    @IBOutlet weak var gameView: UIView!
    @IBOutlet weak var forwardButton: UIButton!
    @IBOutlet weak var backwardButton: UIButton!
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Setting
        ResourceManager.device = MTLCreateSystemDefaultDevice()
        metalLayer = CAMetalLayer()          // 1
        metalLayer.device = ResourceManager.device           // 2
        metalLayer.pixelFormat = .bgra8Unorm // 3
        metalLayer.framebufferOnly = true    // 4
        gameView.layer.addSublayer(metalLayer)   // 5
        
        ResourceManager.commandQueue = ResourceManager.device.makeCommandQueue()
        
        // Rendering
        timer = CADisplayLink(target: self, selector: #selector(ViewController.newFrame(displayLink:)))
        timer.add(to: RunLoop.main, forMode: .default)
        
        registerShaders()
        
        let textureWidth = Int(2 * self.view.bounds.size.width)
        let textureHeight = Int(2 * self.view.bounds.size.height)
        
        ResourceManager.depthBufferDescriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .depth32Float, width: textureWidth, height: textureHeight, mipmapped: false)
        ResourceManager.depthBufferDescriptor.usage = .renderTarget
        ResourceManager.depthTexture = ResourceManager.device.makeTexture(descriptor: ResourceManager.depthBufferDescriptor)
        
        ResourceManager.textureDescriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .bgra8Unorm, width: textureWidth, height: textureHeight, mipmapped: false)
        ResourceManager.textureDescriptor.usage = [.shaderRead, .shaderWrite, .renderTarget]
        
        ResourceManager.textureLoader = MTKTextureLoader(device: ResourceManager.device)
        
        ResourceManager.camera = CameraController()
        
        ResourceManager.projectionMatrix = float4x4(perspectiveProjectionFov: radian(45), aspectRatio: Float(self.view.bounds.size.width / self.view.bounds.size.height), nearZ: NEAR_PLANE, farZ: FAR_PLANE)
        
        ResourceManager.threadgroupSize = MTLSizeMake(16, 16, 1)
        ResourceManager.threadgroupCount = MTLSizeMake(0, 0, 0)
        ResourceManager.threadgroupCount.width  = (textureWidth + ResourceManager.threadgroupSize.width - 1) / ResourceManager.threadgroupSize.width;
        ResourceManager.threadgroupCount.height = (textureHeight + ResourceManager.threadgroupSize.height - 1) / ResourceManager.threadgroupSize.height;
        ResourceManager.threadgroupCount.depth = 1;
        
        scene = Scene(initpos: float3(0))
        scene.generate_scene()
        
        skybox = Skybox()
        skymap = Skymap()
        skymap.draw(sunPos: float3(0.0, 0.5, -1.0))
        
        var model = float4x4(translationBy: -8.0 * PARLIGHT_DIR) * float4x4(scaleBy: 25.0)
        sun = Sun(device: ResourceManager.device, modelMatrix: model, forResourse: "polyball", withExtension: "obj")
    }
    
    override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        if let window = view.window {
            let scale = window.screen.nativeScale
            let layerSize = view.bounds.size
            
            view.contentScaleFactor = scale
            metalLayer.frame = CGRect(x: 0, y: 0, width: layerSize.width, height: layerSize.height)
            metalLayer.drawableSize = CGSize(width: layerSize.width * scale, height: layerSize.height * scale)
        }
        
        ResourceManager.projectionMatrix = float4x4(perspectiveProjectionFov: radian(45), aspectRatio: Float(self.view.bounds.size.width / self.view.bounds.size.height), nearZ: NEAR_PLANE, farZ: FAR_PLANE)
    }
    
    @objc func newFrame(displayLink: CADisplayLink) {
        
        if lastFrameTimestamp == 0.0
        {
            lastFrameTimestamp = displayLink.timestamp
        }
        
        let elapsed: CFTimeInterval = displayLink.timestamp - lastFrameTimestamp
        lastFrameTimestamp = displayLink.timestamp
        
        gameloop(timeSinceLastUpdate: elapsed)
    }
    
    func gameloop(timeSinceLastUpdate: CFTimeInterval) {
        
        scene.updateChunks()
        scene.timeInterval += timeSinceLastUpdate
        
        handleButtonAction()
        
        autoreleasepool {
            self.render()
        }
    }
    
    func render() {
        guard let drawable = metalLayer?.nextDrawable() else { return }
        ResourceManager.commandBuffer = ResourceManager.commandQueue.makeCommandBuffer()!
        
        skybox.draw(drawable: drawable, skymap: skymap, viewMatrix: ResourceManager.camera.viewMatrix, projectionMatrix: ResourceManager.projectionMatrix)
        sun.draw(drawable: drawable)
        scene.draw(drawable: drawable, viewMatrix: ResourceManager.camera.viewMatrix, projectionMatrix: ResourceManager.projectionMatrix ,clearColor: nil)
        
        ResourceManager.commandBuffer.present(drawable)
        ResourceManager.commandBuffer.commit()
    }
    
    func registerShaders() {
        ResourceManager.scenePipelineState = buildShaders(vertexFunction: "sceneVertex", fragmentFunction: "sceneFragment", depth: true, blend: false)
        ResourceManager.instanceScenePipelineState = buildShaders(vertexFunction: "instanceSceneVertex", fragmentFunction: "instanceSceneFragment", depth: true, blend: false)
        ResourceManager.waterPipelineState = buildShaders(vertexFunction: "waterVertex", fragmentFunction: "waterFragment", depth: true, blend: true)

        let defaultLibrary = ResourceManager.device.makeDefaultLibrary()!
        let kernelFunction = defaultLibrary.makeFunction(name: "skymapCompute")!
        ResourceManager.skymapPipelineState = try! ResourceManager.device.makeComputePipelineState(function: kernelFunction)
        
        ResourceManager.skyboxPipelineState = buildShaders(vertexFunction: "skyboxVertex", fragmentFunction: "skyboxFragment", depth: false, blend: false)
    }
    
    func buildShaders(vertexFunction: String, fragmentFunction: String, depth: Bool, blend: Bool) -> MTLRenderPipelineState {
        let defaultLibrary = ResourceManager.device.makeDefaultLibrary()!
        let vertexProgram = defaultLibrary.makeFunction(name: vertexFunction)
        let fragmentProgram = defaultLibrary.makeFunction(name: fragmentFunction)
        
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.vertexFunction = vertexProgram
        pipelineStateDescriptor.fragmentFunction = fragmentProgram
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = .bgra8Unorm
        if depth == true {
            pipelineStateDescriptor.depthAttachmentPixelFormat = .depth32Float
        }
        if blend == true {
            pipelineStateDescriptor.colorAttachments[0].isBlendingEnabled = true
            pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = .sourceAlpha
            pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = .oneMinusSourceAlpha
        }
        
        let pipelineState = try! ResourceManager.device.makeRenderPipelineState(descriptor: pipelineStateDescriptor)
        return pipelineState
    }
    
    func handleButtonAction() {
        
        if forwardButton.isHighlighted {
            ResourceManager.camera.move(.forward)
        }
        if backwardButton.isHighlighted {
            ResourceManager.camera.move(.backward)
        }
        if leftButton.isHighlighted {
            ResourceManager.camera.move(.left)
        }
        if rightButton.isHighlighted {
            ResourceManager.camera.move(.right)
        }

    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        if trackedTouch == nil {
            if let newlyTrackedTouch = touches.first {
                trackedTouch = newlyTrackedTouch
                let point = newlyTrackedTouch.location(in: view)
                ResourceManager.camera.startedInteraction(at: point)
            }
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let previouslyTrackedTouch = trackedTouch {
            if touches.contains(previouslyTrackedTouch) {
                let point = previouslyTrackedTouch.location(in: view)
                ResourceManager.camera.dragged(to: point)
            }
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let previouslyTrackedTouch = trackedTouch {
            if touches.contains(previouslyTrackedTouch) {
                self.trackedTouch = nil
            }
        }
    }
    
    override func touchesCancelled(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let previouslyTrackedTouch = trackedTouch {
            if touches.contains(previouslyTrackedTouch) {
                self.trackedTouch = nil
            }
        }
    }
}

