#version 450
out vec4 FragColor;

in vec2 UV;

uniform sampler2D _Texture;
uniform float radius;

vec3 cameraPos;

void main()
{
vec3 c = texture(_Texture,UV).rgb;
	FragColor = vec4(c,1);
}