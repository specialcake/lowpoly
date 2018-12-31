
import QuartzCore
import simd

enum Camera_Movement {
    case forward, backward, left, right
}

class CameraController {
    var viewMatrix: float4x4 {
        return lookAt(position, position + front, up)
    }
    
    var position: float3 = float3(0, 0, 3)
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
    
    func lookAt(_ eye: float3, _ center: float3, _ up: float3) -> float4x4 {
        let f = normalize(center - eye)
        let s = normalize(cross(f, up))
        let u = cross(s, f)
        
        var result = float4x4(1)
        result[0][0] = s.x
        result[1][0] = s.y
        result[2][0] = s.z
        result[0][1] = u.x
        result[1][1] = u.y
        result[2][1] = u.z
        result[0][2] = -f.x
        result[1][2] = -f.y
        result[2][2] = -f.z
        result[3][0] = -dot(s, eye)
        result[3][1] = -dot(u, eye)
        result[3][2] = dot(f, eye)
        return result;
    }
}
