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

struct Material
{
	float ambientK;
	float diffuseK;
	float specular;
	float shininess;
};

uniform sampler2D _Texture;
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];
uniform Material _Material;
vec3 cameraPos;

void main(){

	int i;
	for(i = 0; i < MAX_LIGHTS; i++)
	{
	vec3 lightColor = vec3(0);
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 pos = normalize( _Lights[i].position - fs_in.WorldPosition);
	vec3 c = texture(_Texture,fs_in.UV).rgb;

	vec3 ambient = _Lights[i].color * _Material.ambientK;
	lightColor += ambient;

	vec3 diffuse = _Lights[i].color * _Material.diffuseK * max(dot(normal, pos),0);
	lightColor += diffuse;

	vec3 h = normalize(pos + cameraPos);
	vec3 specularReflection = _Lights[i].color * _Material.specular * max(pow(dot(h,normal),0),_Material.shininess);
	lightColor += specularReflection;

	c = c * lightColor;
	FragColor = vec4(c,1);
	}
}