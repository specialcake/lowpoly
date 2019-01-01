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
    var device: MTLDevice! = nil
    var metalLayer: CAMetalLayer! = nil
    var commandQueue: MTLCommandQueue!
    
    // Rendering
    var timer: CADisplayLink! = nil
    var lastFrameTimestamp: CFTimeInterval = 0.0
    
    // PipelineState
    var scenePipelineState: MTLRenderPipelineState! = nil
    var instanceScenePipelineState: MTLRenderPipelineState! = nil
    
    var textureLoader: MTKTextureLoader! = nil
    
    // Camera
    // var cameraController: CameraController!
    var projectionMatrix: float4x4!
    var trackedTouch: UITouch?
    
    var scene : Scene!
    
    // UI
    @IBOutlet weak var gameView: UIView!
    @IBOutlet weak var forwardButton: UIButton!
    @IBOutlet weak var backwardButton: UIButton!
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Setting
        device = MTLCreateSystemDefaultDevice()
        metalLayer = CAMetalLayer()          // 1
        metalLayer.device = device           // 2
        metalLayer.pixelFormat = .bgra8Unorm // 3
        metalLayer.framebufferOnly = true    // 4
        gameView.layer.addSublayer(metalLayer)   // 5
        
        commandQueue = device.makeCommandQueue()
        
        // Rendering
        timer = CADisplayLink(target: self, selector: #selector(ViewController.newFrame(displayLink:)))
        timer.add(to: RunLoop.main, forMode: .default)
        
        registerShaders()
        textureLoader = MTKTextureLoader(device: device)
        
        ResourceManager.camera = CameraController()
        projectionMatrix = float4x4(perspectiveProjectionFov: radian(45), aspectRatio: Float(self.view.bounds.size.width / self.view.bounds.size.height), nearZ: 0.01, farZ: 100)
        
        scene = Scene(device: device, initpos: float3(0), shader: scenePipelineState, textureLoader: textureLoader)
        scene.mapInstancePipelineState = instanceScenePipelineState
        
        scene.generate_scene()
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
        
        projectionMatrix = float4x4(perspectiveProjectionFov: radian(45), aspectRatio: Float(self.view.bounds.size.width / self.view.bounds.size.height), nearZ: 0.01, farZ: 100)
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
        
        handleButtonAction()
        
        autoreleasepool {
            self.render()
        }
    }
    
    func render() {
        guard let drawable = metalLayer?.nextDrawable() else { return }
        
        scene.draw(commandQueue: commandQueue, drawable: drawable, viewMatrix: ResourceManager.camera.viewMatrix, projectionMatrix: &projectionMatrix ,clearColor: nil)
        
    }
    
    func registerShaders() {
        scenePipelineState = buildShaders(vertexFunction: "sceneVertex", fragmentFunction: "sceneFragment")
        instanceScenePipelineState = buildShaders(vertexFunction: "instanceSceneVertex", fragmentFunction: "instanceSceneFragment")
    }
    
    func buildShaders(vertexFunction: String, fragmentFunction: String) -> MTLRenderPipelineState {
        let defaultLibrary = device.makeDefaultLibrary()!
        let vertexProgram = defaultLibrary.makeFunction(name: vertexFunction)
        let fragmentProgram = defaultLibrary.makeFunction(name: fragmentFunction)
        
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor()
        pipelineStateDescriptor.vertexFunction = vertexProgram
        pipelineStateDescriptor.fragmentFunction = fragmentProgram
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = .bgra8Unorm
        
        let pipelineState = try! device.makeRenderPipelineState(descriptor: pipelineStateDescriptor)
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

