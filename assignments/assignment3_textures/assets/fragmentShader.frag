#version 450
out vec4 FragColor;
in vec2 UV;
uniform float iTime, scale;
uniform sampler2D _TextureA,_NoiseTexture;


void main()
{
	
	float noise = texture(_NoiseTexture,UV).r * (sin(iTime)*0.5);

	vec2 uv = UV + noise * 0.1f;

	vec4 color = texture(_TextureA,uv);

	FragColor = vec4(color);
}