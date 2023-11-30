#version 450
out vec4 FragColor;


uniform sampler2D _Texture;
uniform float radius;

vec3 cameraPos;

void main()
{
	smoothstep(-radius,radius,0);
}