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
	vec3 c = texture(_Texture,fs_in.UV).rgb;
	vec3 totalLight = vec3(0);
	for(i = 0; i < MAX_LIGHTS; i++)
	{
	vec3 lightColor = vec3(0);
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 pos = normalize( _Lights[i].position - fs_in.WorldPosition);
	
	if(_Material.ambientK < 1)
	{
	vec3 ambient = _Lights[i].color * _Material.ambientK;
	lightColor += ambient;
	}

		if(_Material.diffuseK < 1)
	{
	vec3 diffuse = _Lights[i].color * _Material.diffuseK * max(dot(normal, pos),0);
	lightColor += diffuse;
	}

		if(_Material.specular < 1 && _Material.shininess > 2)
	{
	vec3 v = normalize(cameraPos - fs_in.WorldPosition);
	vec3 h = normalize(pos + v);
	vec3 specularReflection = _Lights[i].color * _Material.specular * pow(max(dot(h,normal),0),_Material.shininess);
	lightColor += specularReflection;
	}

	totalLight += lightColor;
	}
	c = c * totalLight;
	FragColor = vec4(c,1);
}