#version 450
out vec4 FragColor;

in vec2 UV;

uniform sampler2D _Texture;
uniform vec3 _ColorA;
uniform vec3 _ColorB;
uniform float iTime;
vec3 cameraPos;

void main()
{
float noise = texture(_Texture,UV).r * (sin(iTime)*0.5);
	vec2 uv = UV + noise * 0.1f;
	float t = texture(_Texture,UV).r;
vec3 c = mix(_ColorA,_ColorB,t);
	FragColor = vec4(c,1);
}