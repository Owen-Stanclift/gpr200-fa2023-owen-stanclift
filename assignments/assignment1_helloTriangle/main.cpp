#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

unsigned int vbo;
float verticies[9] = {
	//x		//y		//z
	-0.5,	 0.5,	0.0, //bottom left
	 0.5,	-0.5,	0.0, //bottom right
	 0.0,	 0.5,	0.0  //top center

};

const char* vertexShaderSource = R"(
#version 450
layout(location = 0) in vec3 vPos;
void main(){
		gl_Position = vec4(vPos,1.0);
}
)";
const char* fragmentShaderSouce = R"(
#version 450
out vec4 FragColor
void main(){
		FragColor = vec4(1.0);
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
	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Definie position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	//Creates a new vertex array object with vertex data
	unsigned int createVAO(float* vertexData, int numVerticies);

	//-------Create a shader program--------


	//Create a new vertex shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Supply the shader object with source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the shader object
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to comple shader: %s", infoLog);
	}
	//Creates a new shader of a given type.
	//Returns id of the shader object
	unsigned int createShader(GLenum shaderType, const char* sourceCode);

	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSouce);
	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	//Check for linking errors
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	//Delete intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Create a new shader program with vertex + fragment stages
	//Return id of new shader program if successful, 0 if failed
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSouce);

	//-----------Render Loop-----------------

	//----------Multiple Vertex Attributes--------

	//---------Uniforms-----------------

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
