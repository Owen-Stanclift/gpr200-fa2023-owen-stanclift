#version 450
layout(location = 0) in vec3 vCharPos;
layout(location = 1) in vec2 vCharUV;

out vec2 charUV;

uniform float scale,iTime;


void main(){
	
	charUV = vCharUV;
	gl_Position =  vec4(vCharPos*scale + 0.5* cos(iTime) * 0.5,1.0);
	
}