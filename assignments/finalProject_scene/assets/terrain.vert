////Glenn Sakowicz
#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

out vec3 FragPos;
out float height;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	height = position.y;
}
