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
	UV = vUV;
	float t = smoothstep(-_radius,_radius,vPos.y);
	float offset = (sin((vPos.x) + _time*4)*t)/4;
	vec3 position = vec3(vPos.x,vPos.y+offset,vPos.z);
	gl_Position = _ViewProjection * _Model * vec4(position,1.0);
}