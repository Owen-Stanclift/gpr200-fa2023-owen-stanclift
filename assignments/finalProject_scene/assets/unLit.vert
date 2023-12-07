#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec2 UV;

uniform mat4 _Model;
uniform mat4 _ViewProjection;
uniform float _radius;
uniform float _time;
uniform float _speed;
uniform float _strength;
uniform float _frequency;
uniform sampler2D _Texture;

void main(){
	UV = vUV;
	vec3 worldPos = vec3(_Model * vec4(vPos,1.0));
	float t = smoothstep(-_radius,_radius,vPos.y);
	vec2 hUV = vec2(_frequency*worldPos.x + _time * _speed);
	float h = texture(_Texture, hUV).r;
	worldPos.y += (h *_strength*t);
	gl_Position = _ViewProjection * vec4(worldPos,1.0);
}