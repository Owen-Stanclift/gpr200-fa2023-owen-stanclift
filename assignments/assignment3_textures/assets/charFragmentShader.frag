#version 450
out vec4 FragColor;
in vec2 charUV;

uniform sampler2D _Texture;

uniform float iTime;
void main()
{

	vec4 col= texture(_Texture,charUV);

	col.a *= 0.5* cos(iTime) + 0.5;
	
	FragColor = vec4(col);


}