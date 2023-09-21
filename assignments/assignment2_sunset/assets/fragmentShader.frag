	#version 450
	out vec4 FragColor;
	in vec2 UV;
	uniform float iTime;
	uniform float speed;
	float circleSDF(vec2 p, float r)
		{
			 return length(p)-r;
		}
	void main(){
	
			//Sun
			vec2 circlePos = vec2(cos(iTime*speed),sin(iTime*speed));
		float circleRadius = 0.3;
		float d = circleSDF(UV - circlePos,circleRadius);
		d = smoothstep(-0.05,0.05,d);
		vec3 sunColor = vec3(1.0f,1.0f,0.5f);

			//Sky
			float edge = sin(iTime)*0.5 + 0.5;
			float t = smoothstep(edge-0.05,edge+0.05,UV.y);
			vec3 skyColor = mix(vec3(0.0,0.0,1.0),vec3(1.0,0.0,1.0),UV.y);
		//Mountain
		float mountain = ((sin(UV.x*20)) - abs(UV.x*10))*0.1+0.7;
		mountain = step(mountain,UV.y);
		skyColor = mix(vec3(0.2),skyColor,mountain);
		vec3 mountainColor = vec3(0.5,0.5,0.5);
		//FragColor = vec4(mountainColor,1.0); //Mountain
		FragColor = vec4(sunColor,0.5); //Sun
		//FragColor = vec4(color,1.0);//Sky

		
							
	}
	
