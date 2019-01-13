#version 330 core
layout (location = 0) in vec2 aPos;

out VS_OUT{
    vec2 FragPos;
    vec2 TexCoords;
    vec4 cameraRay;
} vs_out;

uniform mat4 invProj;

void main() {
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	vs_out.TexCoords = aPos * 0.5f + 0.5f;
	vs_out.FragPos = aPos;

	vs_out.cameraRay = vec4(aPos, 1.0, 1.0);
	vs_out.cameraRay = invProj * vs_out.cameraRay;
	vs_out.cameraRay = vs_out.cameraRay / vs_out.cameraRay.w;
}
