#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
}fs_in;

struct Light
{
 vec3 position;
 vec3 color;
};

uniform sampler2D _Texture;
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

void main(){
	FragColor = texture(_Texture,fs_in.UV);
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 lighting = dot(normal);
}