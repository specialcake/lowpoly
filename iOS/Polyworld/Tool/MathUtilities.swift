import simd

extension float4 {
    var xyz: float3 {
        return float3(x, y, z)
    }
}

extension float4x4 {
    init(scaleBy s: Float) {
        self.init(float4(s, 0, 0, 0),
                  float4(0, s, 0, 0),
                  float4(0, 0, s, 0),
                  float4(0, 0, 0, 1))
    }
    
    init(rotationAbout axis: float3, by angleRadians: Float) {
        let a = normalize(axis)
        let x = a.x, y = a.y, z = a.z
        let c = cosf(angleRadians)
        let s = sinf(angleRadians)
        let t = 1 - c
        self.init(float4( t * x * x + c,     t * x * y + z * s, t * x * z - y * s, 0),
                  float4( t * x * y - z * s, t * y * y + c,     t * y * z + x * s, 0),
                  float4( t * x * z + y * s, t * y * z - x * s,     t * z * z + c, 0),
                  float4(                 0,                 0,                 0, 1))
    }
    
    init(translationBy t: float3) {
        self.init(float4(   1,    0,    0, 0),
                  float4(   0,    1,    0, 0),
                  float4(   0,    0,    1, 0),
                  float4(t[0], t[1], t[2], 1))
    }
    
    init(perspectiveProjectionFov fovRadians: Float, aspectRatio aspect: Float, nearZ: Float, farZ: Float) {
        let yScale = 1 / tan(fovRadians * 0.5)
        let xScale = yScale / aspect
        let zRange = farZ - nearZ
        let zScale = -(farZ + nearZ) / zRange
        let wzScale = -2 * farZ * nearZ / zRange
        
        let xx = xScale
        let yy = yScale
        let zz = zScale
        let zw = Float(-1)
        let wz = wzScale
        
        self.init(float4(xx,  0,  0,  0),
                  float4( 0, yy,  0,  0),
                  float4( 0,  0, zz, zw),
                  float4( 0,  0, wz,  1))
    }
    
    init(orthoProjectionLeft left: Float, right: Float, bottom: Float, top: Float, near: Float, far: Float)
    {        
        self.init(float4( 2 / (right - left), 0, 0, 0),
                  float4( 0, 2 / (top - bottom), 0, 0),
                  float4( 0, 0, -2 / (far - near), 0),
                  float4( -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1))
    }
    
    var normalMatrix: float3x3 {
        let upperLeft = float3x3(self[0].xyz, self[1].xyz, self[2].xyz)
        return upperLeft.transpose.inverse
    }
    
    static let numberOfElements = 16
}

func radian(_ degree: Float) -> Float
{
    return degree / 180 * .pi
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

func PointToSegment(_ p: float3, _ a: float3, _ b: float3) -> Float {
    let cross = dot(p - a, b - a)
    if (cross <= 0) {
        return length(p - a)
    }
    let ABlen = dot(b - a, b - a)
    if (cross >= ABlen) {
        return length(p - b)
    }
    let Q = cross / ABlen * (b - a)
    return length(p - Q);
}

func sign(val: Float) -> Int {
    return val > 0 ? 1 : val < 0 ? -1 : 0
}

//if p in triangle(a,b,c), p, a, b, c are in same plane
func inTriangle(_ p: float3, _ a: float3, _ b: float3, _ c: float3) -> Bool {
    let sign1 = cross(a - p, b - p)
    let sign2 = cross(b - p, c - p)
    let sign3 = cross(c - p, a - p)
    return dot(sign1, sign2) >= 0 && dot(sign2, sign3) >= 0
}
//point p to triangle(a,b,c)
func distance(_ p: float3, _ a: float3, _ b: float3, _ c: float3) -> Float {
    let V = abs(dot(cross(a - p, b - p), c - p)) / 6.0
    let S = length(cross(a - c, b - c)) / 2.0
    let h = V / S * 3.0
    let Normal = normalize(cross(c - a, b - a));
    let Q = p - Normal * h;
    if(inTriangle(Q, a, b, c)) {
        return h
    }
    let len1 = PointToSegment(Q, a, b)
    let len2 = PointToSegment(Q, b, c);
    let len3 = PointToSegment(Q, c, a);
    let minlen = min(len1, min(len2, len3));
    return sqrt(minlen * minlen + h * h);
}
