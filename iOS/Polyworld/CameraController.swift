
import QuartzCore
import simd

enum Camera_Movement {
    case forward, backward, left, right
}

class CameraController {
    var viewMatrix: float4x4 {
        return lookAt(position, position + front, up)
    }
    
    var position: float3 = float3(0, 5, 3)
    var up: float3 = float3(0, 1, 0)
    var front: float3 = float3(0, 0, -1)
    var right: float3 = float3(1, 0, 0)
    var worldUp: float3 = float3(0, 1, 0)
    
    var yaw: Float = -90
    var pitch: Float = 0
    
    var sensitivity: Float = 0.2
    var velocity: Float = 0.01
    let minPitch: Float = -90
    let maxPitch: Float = 90

    private var lastPoint: CGPoint = .zero
    
    func startedInteraction(at point: CGPoint) {
        lastPoint = point
    }
    
    func dragged(to point: CGPoint) {
        let deltaX = Float(point.x - lastPoint.x)
        let deltaY = Float(point.y - lastPoint.y)
        yaw += deltaX * sensitivity
        pitch -= deltaY * sensitivity
        pitch = min(max(minPitch, pitch), maxPitch)
        lastPoint = point
        
        updateupdateCameraVectors()
    }
    
    func updateupdateCameraVectors() {
        var newFront: float3 = float3()
        newFront.x = cos(radian(yaw)) * cos(radian(pitch))
        newFront.y = sin(radian(pitch))
        newFront.z = sin(radian(yaw)) * cos(radian(pitch))
        front = normalize(newFront)
        right = normalize(cross(front, worldUp))
        up = normalize(cross(right, front))
    }
    
    func move(_ direction: Camera_Movement) {
        switch direction {
        case .forward:
            position += front * velocity
        case .backward:
            position -= front * velocity
        case .left:
            position -= right * velocity
        case .right:
            position += right * velocity
        }
    }
}
