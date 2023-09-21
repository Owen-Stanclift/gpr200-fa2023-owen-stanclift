	#version 450
	out vec4 FragColor;
	in vec2 UV;
	void main(){
		FragColor = vec4(UV,1.0,0.0);
	}