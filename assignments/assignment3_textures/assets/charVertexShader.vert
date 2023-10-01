#version 450
layout(location = 0) in vec3 vCharPos;
layout(location = 1) in vec2 vCharUV;

out vec2 UV;

uniform float scale;


void main(){
	
	UV = vCharUV;
	gl_Position =  vec4(vCharPos,1.0);
	
}