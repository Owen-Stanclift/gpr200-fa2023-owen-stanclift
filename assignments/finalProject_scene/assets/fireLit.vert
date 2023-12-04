#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec2 UV;

uniform mat4 _Model;
uniform mat4 _ViewProjection;
uniform float _radius;
uniform float _time;
void main(){
	float t = smoothstep(-_radius,_radius,vPos.y);
	float offset = sin(vPos.x + _time)*t;
	gl_Position = _ViewProjection * vec4(vNormal,1.0);
}