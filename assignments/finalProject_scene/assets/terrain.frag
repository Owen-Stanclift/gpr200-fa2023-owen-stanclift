////Glenn Sakowicz
#version 450 core

in vec3 FragPos;
in float height;

uniform vec3 colorLow;
uniform vec3 colorHigh;

out vec4 FragColor;

void main()
{
	vec3 color = mix(colorLow, colorHigh, (height + 5.0)/10.0);
	FragColor = vec4(color, 1.0);
}