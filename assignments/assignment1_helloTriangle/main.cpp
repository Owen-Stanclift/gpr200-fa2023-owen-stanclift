#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

unsigned int vbo;
unsigned int createVAO(float* vertexData, int numVerticies);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createShader(GLenum shaderType, const char* sourceCode);

float verticies[21] = {
	//x		//y		//z		//r		//g		//b		//a
	-0.5,	 0.5,	0.0,	1.0,	0.0,	0.0,	1.0,//bottom left
	 0.5,	-0.5,	0.0,	0.0,	1.0,	0.0,	1.0,//bottom right
	 0.0,	 0.5,	0.0,	0.0,	0.0,	1.0,	1.0//top center

};

const char* vertexShaderSource = R"(
#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vColor;
out vec4 Color;
uniform float _Time;
void main(){
		Color = vColor;
		vec3 offset = vec3(0,sin(vPos.x + _Time),0)*0.5;
		gl_Position = vec4(vPos+ offset,1.0);
}
)";
const char* fragmentShaderSource = R"(
#version 450
out vec4 FragColor;
in vec4 Color;
uniform float _Time;
void main(){
		FragColor = Color * abs(sin(_Time));
}

)";

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}


	//--------Vertex Data----------


	

	

	//Creates a new vertex array object with vertex data
	

	//-------Create a shader program--------


	//Create a new vertex shader object
	
	//Creates a new shader of a given type.
	//Returns id of the shader object
	

	

	//Create a new shader program with vertex + fragment stages
	//Return id of new shader program if successful, 0 if failed


	//-----------Render Loop-----------------
	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(verticies,3);

	//The current time in seconds this frame
	
	


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		float time = (float)glfwGetTime();
		//Get location of the uniform by name
		int timeLocation = glGetUniformLocation(shader, "_Time");
		glUniform1f(timeLocation, time);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniform1f(timeLocation, time);
		glDrawArrays(GL_TRIANGLES, 0,3);
		glfwSwapBuffers(window);
	}

	printf("Shutting down...");
}

unsigned int createVAO(float* vertexData, int numVerticies)
{
	//Define a new buffer id
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * numVerticies, vertexData, GL_STATIC_DRAW);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Definie position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	//Color Attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	return vao;
}
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);
	int success;
	//Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return 0;
	}
	//Delete intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	unsigned int shader = glCreateShader(shaderType);
	//Supply the shader object with source code
	glShaderSource(shader, 1, &sourceCode, NULL);
	//Compile the shader object
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to comple shader: %s", infoLog);
		return 0;
	}
	return shader;
}