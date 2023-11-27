#version 450
out vec4 FragColor;

uniform vec3 _Color;
uniform sampler2D sprite;
uniform vec2 _TexCords;

void main(){

	
	FragColor = (texture(sprite,_TexCords)* vec4(_Color,1.0));
}
