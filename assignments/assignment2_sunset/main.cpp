#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <myLib/shader.h>


unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex vertices[4] = {
	//x   //y  //z   //u	//v
	{-1, -1, 0.0, 0.0,	0.0}, //Bottom left
	{1, -1, 0.0, 1.0,	0.0 },//Bottom right
	{-1,  1, 0.0, 0.0,	1.0}, //Top left
	{1,  1,	0.0, 1.0,	1.0} // Top right
};

unsigned int indicies[6] = {
	0,1,3,
	0,3,2
};

const char* fragmentShaderSource;
const char* vertexShaderSource;


//background
float triangleColor[3] = { 0.0f, 0.5f, 1.0f };
float triangleBrightness = 1.0f;
//mountain
float mountainColor[3] = {0.5f,0.5f,0.5f};
float mountainBrightness = 1.0f;

float skyColor1[3] = {0.0f, 0.0f, 1.0f};
float skyColor2[3] = {0.0f, 0.5f, 1.0f};

float sunColor[3] = {1.0f, 1.0f, 0.5f};
float sunRadius = 0.3f;
bool showImGUIDemoWindow = true;

float speed =1;

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicies, int numIndicies);

int main() {

	
	
	/*unsigned int shader = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());*/
	
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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	yourLib::Shader shader("assets/vertexShader.vert" , "assets/fragmentShader.frag");
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	/*unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);*/
	unsigned int vao = createVAO(vertices, 4,indicies,6);

	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		float time = (float)glfwGetTime();
		//Set uniforms
		/*glUniform3f(glGetUniformLocation(shader, "_Color"), triangleColor[0], triangleColor[1], triangleColor[2]);*/
		shader.setVec3((shader, "_Color"), triangleColor[0], triangleColor[1], triangleColor[2]);
		/*glUniform1f(glGetUniformLocation(shader,"_Brightness"), triangleBrightness);*/
		shader.setFloat((shader, "_Brightness"), triangleBrightness);
		shader.setFloat("iTime", time);
		shader.setFloat("speed", speed);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color", triangleColor);
			ImGui::SliderFloat("Sky Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::ColorEdit3("Sky Color 1", skyColor1);
			ImGui::ColorEdit3("Sky Color 2", skyColor2);
			ImGui::ColorEdit3("Mountain Color", mountainColor);
			ImGui::SliderFloat("Mountain Brightness", &mountainBrightness, 0.0f, 1.0f);
			ImGui::ColorEdit3("Sun Color ", sunColor);
			ImGui::SliderFloat("Sun Brightness", &sunRadius, 0.0f, 1.0f);
			ImGui::SliderFloat("Sun Speed", &speed, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}




unsigned int createVAO(Vertex* vertexData, int numVertices,unsigned int* indiceisData, int numIndicies) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndicies, indiceisData, GL_STATIC_DRAW);
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

