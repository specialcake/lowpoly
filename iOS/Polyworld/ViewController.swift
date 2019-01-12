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
import CoreMotion

class ViewController: UIViewController {
    
    // Setting
    var metalLayer: CAMetalLayer! = nil
    
    // Rendering
    var timer: CADisplayLink! = nil
    var lastFrameTimestamp: CFTimeInterval = 0.0

    var trackedTouch: UITouch?
    
    var lightSpaceMatrix: float4x4!
    
    var scene: Scene!
    var skymap: Skymap!
    var skybox: Skybox!
    var shadowmap: Shadowmap!
    var sun: Sun!
    var polyball: Polyball!
    
    //运动管理器
    let motionManager = CMMotionManager()
    
    //刷新时间间隔
    let timeInterval: TimeInterval = 0.01
    var isFollowed: Bool = true
    
    // UI
    @IBOutlet weak var gameView: UIView!
    @IBOutlet weak var forwardButton: UIButton!
    @IBOutlet weak var backwardButton: UIButton!
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    @IBOutlet weak var jumpButton: UIButton!
    @IBOutlet weak var followButton: UIButton!
    
    
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
        
        registerShaders()
        
        let textureWidth = Int(2 * self.view.bounds.size.width)
        let textureHeight = Int(2 * self.view.bounds.size.height)
        
        ResourceManager.depthBufferDescriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .depth32Float, width: textureWidth, height: textureHeight, mipmapped: false)
        ResourceManager.depthBufferDescriptor.usage = [.shaderRead, .shaderWrite, .renderTarget]
        ResourceManager.depthTexture = ResourceManager.device.makeTexture(descriptor: ResourceManager.depthBufferDescriptor)
        
        ResourceManager.shadowmapBufferDescriptor = MTLTextureDescriptor.texture2DDescriptor(pixelFormat: .depth32Float, width: 1000, height: 1000, mipmapped: false)
        ResourceManager.shadowmapBufferDescriptor.usage = [.shaderRead, .shaderWrite, .renderTarget]
        ResourceManager.shadowmapDepthTexture = ResourceManager.device.makeTexture(descriptor: ResourceManager.shadowmapBufferDescriptor)!
        
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
        shadowmap = Shadowmap()
        
        let sunModel = float4x4(translationBy: -8.0 * PARLIGHT_DIR) * float4x4(scaleBy: 25.0)
        sun = Sun(device: ResourceManager.device, modelMatrix: sunModel, forResourse: "polyball", withExtension: "obj")
        
        let polyballModel = float4x4(translationBy: float3(0.0, 0.4, 4.0))
        polyball = Polyball(device: ResourceManager.device, modelMatrix: polyballModel, forResourse: "polyball", withExtension: "obj")
        
        // Rendering
        timer = CADisplayLink(target: self, selector: #selector(ViewController.newFrame(displayLink:)))
        timer.add(to: RunLoop.main, forMode: .default)
        
        //开始陀螺仪更新
        startGyroUpdates()
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
        
        shadowmap.updateFrustum(scene: scene)
        
        lightSpaceMatrix = shadowmap.getlightSpaceMatrix(scene: scene)
        scene.Generate_ShadowMap(lightSpaceMatrix: lightSpaceMatrix)
        
        handleButtonAction()
        
        polyball.Cam.Front = ResourceManager.camera.front
        polyball.Cam.Right = ResourceManager.camera.right
        polyball.Cam.Up = ResourceManager.camera.up
        
        polyball.UpdateSpeed(deltaTime: Float(timeSinceLastUpdate))
        polyball.UpdatePosition(deltaTime: Float(timeSinceLastUpdate), scene: scene)
        
        if isFollowed {
            ResourceManager.camera.position = polyball.GenCameraPosition()
        }
        
        autoreleasepool {
            self.render()
        }
    }
    
    func render() {
        guard let drawable = metalLayer?.nextDrawable() else { return }
        ResourceManager.commandBuffer = ResourceManager.commandQueue.makeCommandBuffer()!

        skybox.draw(drawable: drawable, skymap: skymap, viewMatrix: ResourceManager.camera.viewMatrix, projectionMatrix: ResourceManager.projectionMatrix)
        sun.draw(drawable: drawable)
        scene.draw(drawable: drawable, viewMatrix: ResourceManager.camera.viewMatrix, projectionMatrix: ResourceManager.projectionMatrix, lightSpaceMatrix: lightSpaceMatrix, shadowmap: ResourceManager.shadowmapDepthTexture)
        polyball.draw(drawable: drawable)
        
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
        ResourceManager.shadowmapPipelineState = buildShaders(vertexFunction: "shadowmapVertex", fragmentFunction: "shadowmapFragment", depth: true, blend: false)
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
            if isFollowed {
                ResourceManager.keys[BallMovement.forward.rawValue] = true
            }
            else {
                ResourceManager.camera.move(.forward)
            }
        } else {
            ResourceManager.keys[BallMovement.forward.rawValue] = false
        }
        if backwardButton.isHighlighted {
            if isFollowed {
                ResourceManager.keys[BallMovement.backward.rawValue] = true
            }
            else {
                ResourceManager.camera.move(.backward)
            }
        } else {
            ResourceManager.keys[BallMovement.backward.rawValue] = false
        }
        if leftButton.isHighlighted {
            if isFollowed {
                ResourceManager.keys[BallMovement.left.rawValue] = true
            }
            else {
                ResourceManager.camera.move(.left)
            }
        } else {
            ResourceManager.keys[BallMovement.left.rawValue] = false
        }
        if rightButton.isHighlighted {
            if isFollowed {
                ResourceManager.keys[BallMovement.right.rawValue] = true
            }
            else {
                ResourceManager.camera.move(.right)
            }
        } else {
            ResourceManager.keys[BallMovement.right.rawValue] = false
        }
        if jumpButton.isHighlighted && isFollowed {
            ResourceManager.keys[BallMovement.jump.rawValue] = true
        } else {
            ResourceManager.keys[BallMovement.jump.rawValue] = false
        }
    }
    
    @IBAction func followButtonTapped(_ sender: UIButton) {
        isFollowed = !isFollowed
        if isFollowed {
            followButton.setImage(UIImage(named: "跟随.png"), for: .normal)
            followButton.setImage(UIImage(named: "跟随_按下.png"), for: .highlighted)
        } else {
            followButton.setImage(UIImage(named: "不跟随.png"), for: .normal)
            followButton.setImage(UIImage(named: "不跟随_按下.png"), for: .highlighted)
        }
    }
    
    @IBAction func screenshotButtonTapped(_ sender: Any) {
        guard let window = UIApplication.shared.keyWindow else { return }
        
        // 用下面这行而不是UIGraphicsBeginImageContext()，因为前者支持Retina
        UIGraphicsBeginImageContextWithOptions(window.bounds.size, false, 0.0)
        
        window.layer.render(in: UIGraphicsGetCurrentContext()!)
        
        let image = UIGraphicsGetImageFromCurrentImageContext()
        
        UIGraphicsEndImageContext()
        
        let activityController = UIActivityViewController(activityItems: [image], applicationActivities: nil)
        activityController.popoverPresentationController?.sourceView = self.view
        present(activityController, animated: true, completion: nil)
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
    
    // 开始获取陀螺仪数据
    func startGyroUpdates() {
        //判断设备支持情况
        guard motionManager.isGyroAvailable else { return }
        
        //设置刷新时间间隔
        self.motionManager.gyroUpdateInterval = self.timeInterval
        
        //开始实时获取数据
        let queue = OperationQueue.current
        self.motionManager.startGyroUpdates(to: queue!, withHandler: { (gyroData, error) in
            guard error == nil else {
                print(error!)
                return
            }
            // 有更新
            if self.motionManager.isGyroActive {
                if let rotationRate = gyroData?.rotationRate {
                    ResourceManager.camera.rotate(rotationRate: rotationRate)
                }
            }
        })
    }
}

