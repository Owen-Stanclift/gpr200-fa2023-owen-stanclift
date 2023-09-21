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
			
		
			//Bright Sky 
			float blend = sin(iTime)*0.5 + 0.5;
			vec3 color1 = vec3(blend,blend/2,0.0f);
			vec3 color2 = vec3(0.0f,0.0f,blend);
			vec3 color = mix(color1,color2,UV.y);//Blue sky
			

			//Dark Sky
			float edge = sin(iTime)*0.5 + 0.5;
			float t = smoothstep(edge+0.05,edge-0.05,UV.y); //Dark sky going up and down
			
			vec3 skyColor = vec3(0.0f,0.0f,t);
			color = mix(color,skyColor,UV.y);
				//Sun
			vec2 circlePos = vec2(cos(iTime*speed),sin(iTime*speed));
		float circleRadius = 0.3;
		float d = circleSDF(UV - circlePos,circleRadius);
		d = smoothstep(-0.05,0.05,d);
		vec3 sunColor = vec3(1.0f,1.0f,0.5f);
		color = mix(sunColor,color,d);
		//Mountain

		float mountain = ((sin(UV.x*20)) - abs(UV.x*10))*0.1+0.7;
		mountain = step(mountain,UV.y);
		color = mix(vec3(0.2),color,mountain);
		vec3 mountainColor = vec3(0.5,0.5,0.5);
		//FragColor = vec4(mountainColor,1.0); //Mountain
		//FragColor = vec4(color,0.5); //Sun
		FragColor = vec4(color,1.0);//Sky

		
							
	}
	
