//
//  ARViewController.swift
//  Polyworld
//
//  Created by 沈心逸 on 2018/11/18.
//  Copyright © 2018年 Sxy. All rights reserved.
//

import UIKit
import SceneKit
import ARKit

struct CollisionCategory {
    let rawValue: Int
    
    static let bottom = CollisionCategory(rawValue: 1 << 0)
    static let cube = CollisionCategory(rawValue: 1 << 1)
}

class ARViewController: UIViewController, ARSCNViewDelegate {
    
    @IBOutlet var sceneView: ARSCNView!
    var planes = [UUID:Plane]() // 字典，存储场景中当前渲染的所有平面
    var boxes = [SCNNode]()
    var texture: Int = 0
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupScene()
        setupRecognizers()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        setupSession()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        // Pause the view's session
        sceneView.session.pause()
    }
    
    func setupScene() {
        // 存放所有 3D 几何体的容器
        let scene = SCNScene()
        
        /*
         // 想要绘制的 3D 立方体
         let boxGeometry = SCNBox(width: 0.1, height: 0.1, length: 0.1, chamferRadius: 0.01)
         
         // 将几何体包装为 node 以便添加到 scene
         let boxNode = SCNNode(geometry: boxGeometry)
         
         // 把 box 放在摄像头正前方
         boxNode.position = SCNVector3Make(0.05, 0, -0.1)
         
         // rootNode 是一个特殊的 node，它是所有 node 的起始点
         scene.rootNode.addChildNode(boxNode)
         */
        
        // 添加一些默认光照
        sceneView.autoenablesDefaultLighting = true
        
        // 添加Debug信息
        sceneView.debugOptions = [ARSCNDebugOptions.showPhysicsShapes, ARSCNDebugOptions.showFeaturePoints]
        
        // 设置 ARSCNViewDelegate——此协议会提供回调来处理新创建的几何体
        sceneView.delegate = self
        
        // 显示统计数据（statistics）如 fps 和 时长信息
        // sceneView.showsStatistics = true
        
        // 将 scene 赋给 view
        sceneView.scene = scene
    }
    
    func setupSession() {
        // 创建 session 配置（configuration）实例
        let configuration = ARWorldTrackingConfiguration()
        
        // 明确表示需要追踪水平面。设置后 scene 被检测到时就会调用 ARSCNViewDelegate 方法
        configuration.planeDetection = .horizontal
        
        // 运行 view 的 session
        sceneView.session.run(configuration)
    }
    
    func setupRecognizers() {
        // 轻点一下就会往场景中插入新的几何体
        let tapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(ARViewController.handleTapFrom(recognizer:) ))
        tapGestureRecognizer.numberOfTapsRequired = 1
        sceneView.addGestureRecognizer(tapGestureRecognizer)
    }
    
    @objc func handleTapFrom(recognizer: UITapGestureRecognizer) {
        // 获取屏幕空间坐标并传递给 ARSCNView 实例的 hitTest 方法
        let tapPoint = recognizer.location(in: sceneView)
        
        // 触碰球
        let results = sceneView.hitTest(tapPoint)
        
        if let node = results.first?.node, node.physicsBody?.type == .dynamic, let coord = results.first?.worldCoordinates {
            let scale: Float = 0.3
            let direction = SCNVector3(
                scale * (node.position.x - coord.x),
                scale * (node.position.y - coord.y),
                scale * (node.position.z - coord.z)
            )
            node.physicsBody?.applyForce(direction, asImpulse: true)
            return
        }
        
        let result = sceneView.hitTest(tapPoint, types: .existingPlaneUsingExtent)
        
        // 如果射线与某个平面几何体相交，就会返回该平面，以离摄像头的距离升序排序
        // 如果命中多次，用距离最近的平面
        if let hitResult = result.first {
            insertGeometry(hitResult)
        }
    }
    
    func insertGeometry(_ hitResult: ARHitTestResult) {
        
        // 现在先插入简单的小方块，后面会让它变得更好玩，有更好的纹理和阴影
//        let dimension: CGFloat = 0.1
//        let cube = SCNBox(width: dimension, height: dimension, length: dimension, chamferRadius: 0)
//        let node = SCNNode(geometry: cube)
        
//        guard let url = Bundle.main.url(forResource: "ARpolyball", withExtension: "obj") else { fatalError()
//        }
//        guard let node = SCNReferenceNode(url: url) else {
//            fatalError("load baby_groot error.")
//        }
        
        let scene = SCNScene(named: "ARpolyball.scn")!
        if let node = scene.rootNode.childNode(withName: "ball", recursively: true) {
            
            //node.load()
            
            // physicsBody 会让 SceneKit 用物理引擎控制该几何体
            let sphere = SCNSphere(radius: 0.1)
            let shape = SCNPhysicsShape(geometry: sphere, options: [SCNPhysicsShape.Option.type: SCNPhysicsShape.ShapeType.convexHull, SCNPhysicsShape.Option.scale: SCNVector3(0.1,0.1,0.1)])
            node.physicsBody = SCNPhysicsBody(type: .dynamic, shape: shape)
            node.physicsBody?.mass = 0.05
            //node.physicsBody?.categoryBitMask = CollisionCategory.cube.rawValue
        
            change(node: node)
            
            // 把几何体插在用户点击的点再稍高一点的位置，以便使用物理引擎来掉落到平面上
            let insertionYOffset: Float = 0.25
            node.position = SCNVector3Make(
                hitResult.worldTransform.columns.3.x,
                hitResult.worldTransform.columns.3.y + insertionYOffset,
                hitResult.worldTransform.columns.3.z
            )
            sceneView.scene.rootNode.addChildNode(node)
            boxes.append(node)
            
        }
    }
    
    @IBAction func deleteGeometry(_ sender: UIBarButtonItem) {
        if !boxes.isEmpty {
            for ball in boxes {
                ball.removeFromParentNode()
            }
        }
    }
    
    @IBAction func changeColor(_ sender: UIBarButtonItem) {
        
        texture = (texture + 1) % 8
    
        if !boxes.isEmpty {
            for ball in boxes {
                change(node: ball)
            }
        }
    }
    
    func change(node: SCNNode) {
        let material = SCNMaterial()
        
        if (texture == 0) {
            material.diffuse.contents = UIColor.white
        } else if (texture == 1) {
            material.diffuse.contents = UIColor(displayP3Red: 0.5, green: 1.0, blue: 0.5, alpha: 1)
        } else if (texture == 2) {
            material.diffuse.contents = UIColor(displayP3Red: 0.0, green: 0.5, blue: 1.0, alpha: 1)
        } else if (texture == 3) {
            material.diffuse.contents = UIColor(displayP3Red: 1.0, green: 0.5, blue: 0.5, alpha: 1)
        } else if (texture == 4) {
            material.diffuse.contents = UIImage(named: "moon.jpg")
        } else if (texture == 5) {
            material.diffuse.contents = UIImage(named: "mars.jpg")
        } else if (texture == 6) {
            material.diffuse.contents = UIImage(named: "sun.jpg")
        } else if (texture == 7) {
            material.diffuse.contents = UIImage(named: "venus.jpg")
        }
        
        node.geometry?.materials = [material]
    }

    
    // MARK: - ARSCNViewDelegate
    
    /**
     实现此方法来为给定 anchor 提供自定义 node。
     
     @discussion 此 node 会被自动添加到 scene graph 中。
     如果没有实现此方法，则会自动创建 node。
     如果返回 nil，则会忽略此 anchor。
     @param renderer 将会用于渲染 scene 的 renderer。
     @param anchor 新添加的 anchor。
     @return 将会映射到 anchor 的 node 或 nil。
     */
//    func renderer(_ renderer: SCNSceneRenderer, nodeFor anchor: ARAnchor) -> SCNNode? {
//        return nil
//    }
    
    override func motionEnded(_ motion: UIEvent.EventSubtype, with event: UIEvent?) {
        if !boxes.isEmpty {
            for ball in boxes {
                ball.physicsBody?.applyForce(SCNVector3(0, 0.1, 0), asImpulse: true)
            }
        }
    }
    
    /**
     将新 node 映射到给定 anchor 时调用。
     
     @param renderer 将会用于渲染 scene 的 renderer。
     @param node 映射到 anchor 的 node。
     @param anchor 新添加的 anchor。
     */
    func renderer(_ renderer: SCNSceneRenderer, didAdd node: SCNNode, for anchor: ARAnchor) {
        guard let anchor = anchor as? ARPlaneAnchor else {
            return
        }
        
        // 检测到新平面时创建 SceneKit 平面以实现 3D 视觉化
        let plane = Plane(withAnchor: anchor)
        planes[anchor.identifier] = plane
        node.addChildNode(plane)
    }
    
    /**
     使用给定 anchor 的数据更新 node 时调用。
     
     @param renderer 将会用于渲染 scene 的 renderer。
     @param node 更新后的 node。
     @param anchor 更新后的 anchor。
     */
    func renderer(_ renderer: SCNSceneRenderer, didUpdate node: SCNNode, for anchor: ARAnchor) {
        guard let plane = planes[anchor.identifier] else {
            return
        }
        
        // anchor 更新后也需要更新 3D 几何体。例如平面检测的高度和宽度可能会改变，所以需要更新 SceneKit 几何体以匹配
        plane.update(anchor: anchor as! ARPlaneAnchor)
    }
    
    /**
     从 scene graph 中移除与给定 anchor 映射的 node 时调用。
     
     @param renderer 将会用于渲染 scene 的 renderer。
     @param node 被移除的 node。
     @param anchor 被移除的 anchor。
     */
    func renderer(_ renderer: SCNSceneRenderer, didRemove node: SCNNode, for anchor: ARAnchor) {
        // 如果多个独立平面被发现共属某个大平面，此时会合并它们，并移除这些 node
        planes.removeValue(forKey: anchor.identifier)
    }
    
    /**
     将要用给定 anchor 的数据来更新时 node 调用。
     
     @param renderer 将会用于渲染 scene 的 renderer。
     @param node 即将更新的 node。
     @param anchor 被更新的 anchor。
     */
    func renderer(_ renderer: SCNSceneRenderer, willUpdate node: SCNNode, for anchor: ARAnchor) {
    }
    
    func session(_ session: ARSession, didFailWithError error: Error) {
        // Present an error message to the user
        
    }
    
    func sessionWasInterrupted(_ session: ARSession) {
        // Inform the user that the session has been interrupted, for example, by presenting an overlay
        
    }
    
    func sessionInterruptionEnded(_ session: ARSession) {
        // Reset tracking and/or remove existing anchors if consistent tracking is required
        
    }
}
