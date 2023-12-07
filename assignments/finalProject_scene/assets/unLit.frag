#version 450
out vec4 FragColor;

in vec2 UV;

uniform sampler2D _Texture;
uniform vec3 _ColorA;
uniform vec3 _ColorB;
uniform float _time;
vec3 cameraPos;

void main()
{
vec2 hUV = UV;
hUV.y -= _time;
float noise = texture(_Texture,UV).r * (sin(_time)*0.5);
	vec2 uv = UV + noise * 0.1f;
	float t = texture(_Texture,UV-vec2(_time)).r;
	float h = texture(_Texture,hUV).r;
	h += 0.5 * 0.5;
	float a = 1.0 - step(h,UV.y);
vec3 c = mix(_ColorA,_ColorB,t);
	FragColor = vec4(c,a);
}