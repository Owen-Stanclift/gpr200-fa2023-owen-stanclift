#version 450
out vec4 FragColor;
in vec2 UV,charUV;
uniform float iTime, scale;
uniform sampler2D _TextureA,_TextureB,_NoiseTexture;


void main()
{
	
	float noise = texture(_NoiseTexture,UV).r * (sin(iTime)*0.5);

	vec2 uv = UV + noise * 0.1f;

	vec4 colorA = texture(_TextureA,uv);
	vec4 colorB = texture(_TextureB,charUV);
	vec3 color = mix(colorA.rgb,colorB.rgb,colorB.a * (0.5* cos(iTime) + 0.5));

	FragColor = vec4(color,UV);
}