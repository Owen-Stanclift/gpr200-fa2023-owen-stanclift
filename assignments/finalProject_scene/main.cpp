#include <stdio.h>
#include <math.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <myLib/procGen.h>
#include <am/texture.h>
#include <gs/terrain.h>
#include <gs/Perlin.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);


int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
int numFlames = 4;
float speed = 1;
float strength = 2;
float frequency = 2;
ew::Vec3 bgColor = ew::Vec3(0.1f);


ew::Camera camera;
ew::CameraController cameraController;

struct Flame
{
	ew::Vec3 position;
	ew::Vec3 color;
};
struct Material
{
	float ambientK;
	float diffuseK;
	float specular;
	float shininess;
};

struct Particle
{
	ew::Vec2 position, velocity;
	ew::Vec3 color;
	float life;
};

struct Vertex {
	float x, y, z;
};

Vertex skyboxVertices[] = {
	{-1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{ 1.0f, -1.0f, -1.0f},
	{ 1.0f, -1.0f, -1.0f},
	{ 1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},

	{-1.0f, -1.0f,  1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f,  1.0f},
	{-1.0f, -1.0f,  1.0f},

	{ 1.0f, -1.0f, -1.0f},
	{ 1.0f, -1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f, -1.0f},
	{ 1.0f, -1.0f, -1.0f},

	{-1.0f, -1.0f,  1.0f},
	{-1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{ 1.0f, -1.0f,  1.0f},
	{-1.0f, -1.0f,  1.0f},

	{-1.0f,  1.0f, -1.0f},
	{ 1.0f,  1.0f, -1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{-1.0f,  1.0f,  1.0f},
	{-1.0f,  1.0f, -1.0f},

	{-1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f},
	{ 1.0f, -1.0f, -1.0f},
	{ 1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f},
	{ 1.0f, -1.0f,  1.0f}
};

unsigned short skyboxIndices[] = {
	//right
	6, 5, 1,
	1, 2, 6,
	//left
	7, 3, 0,
	0, 4, 7,
	//top
	4, 5, 6,
	6, 7, 4,
	//bottom
	0, 3, 2,
	2, 1, 0,
	//front
	6, 2, 3,
	3, 7, 6,
	//back
	5, 4, 0,
	0, 1, 5
};

////Glenn Sakowicz*************
Terrain terrain(50, 1.0f);
PerlinNoise perlin;
int gridSize = 128;
int stepSize = 10;
//*************************
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);

int main()
{
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	// Aylwin Morgan
	std::vector<std::string> faces;
	faces.push_back("assets/right.jpg");
	faces.push_back("assets/left.jpg");
	faces.push_back("assets/top.jpg");
	faces.push_back("assets/bottom.jpg");
	faces.push_back("assets/back.jpg");
	faces.push_back("assets/front.jpg");

	unsigned int skybox = loadTextures(faces);
	unsigned int skyboxVAO = createVAO(skyboxVertices, 36, skyboxIndices, 36);

	ew::Shader skyboxShader("assets/skybox.vert", "assets/skybox.frag");

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//Owen Stanclift
	ew::Shader shader("assets/unLit.vert", "assets/unLit.frag");
	ew::Shader fireShader("assets/fireLit.vert", "assets/fireLit.frag");
	unsigned int noiseTexture = ew::loadTexture("assets/noiseTexture.png", GL_REPEAT, GL_LINEAR);

	Flame flames[4];
	Material material;
	float fireRadius = 0.5f;
	ew::Mesh fireMesh(myLib::createFire(fireRadius, 60, 5));
	ew::Transform* fireTransform = new ew::Transform[numFlames];
	
	fireTransform[0].position = ew::Vec3(1.0, 1.0, 1.0);
	fireTransform[1].position = ew::Vec3(-1.0, 1.0, 1.0);
	fireTransform[2].position = ew::Vec3(1.0, 1.0, -1.0);
	fireTransform[3].position = ew::Vec3(-1.0, 1.0, -1.0);

	ew::Vec3 fireColor1 = ew::Vec3(1.0, 0.0, 0.0);
	ew::Vec3 fireColor2 = ew::Vec3(1.0, 1.0, 0.0);

	material.ambientK = 0.2f;
	material.diffuseK = 0.2f;
	material.specular = 0.2f;
	material.shininess = 3.0f;

	resetCamera(camera, cameraController);

	////Glenn Sakowicz*********************************************
	std::vector<Vertex> vertices;
	for (int i = 0; i < terrain.getVertices().size(); i += 3) 
	{
		Vertex vertex;
		vertex.x = terrain.getVertices()[i];
		vertex.y = terrain.getVertices()[i + 1];
		vertex.z = terrain.getVertices()[i + 2];
		vertices.push_back(vertex);
	}
	std::vector<unsigned short> indices;
	for (unsigned int i : terrain.getIndices())
	{
		indices.push_back(static_cast<unsigned short>(i));
	}

	ew::Shader terrainShader("assets/terrain.vert", "assets/terrain.frag");
	unsigned int terrainVAO = createVAO(vertices.data(), vertices.size()*3, indices.data(), indices.size());
	//*********************************************************************
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();


		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		// Aylwin Morgan
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);
		skyboxShader.use();
		skyboxShader.setMat4("projection", camera.ProjectionMatrix());
		skyboxShader.setMat4("view", camera.ViewMatrix());

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		//Owen Stanclift
		fireShader.use();
		fireShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		for (int i = 0; i < numFlames; i++)
		{
			flames[i].position = flames[i].position;
			fireShader.setMat4("_Model", fireTransform[i].getModelMatrix());
			fireShader.setVec3("_Color", flames[i].color);
		}


		shader.use();
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setFloat("_time", time);
		shader.setFloat("_speed", speed);
		shader.setFloat("_strength", strength);
		shader.setFloat("_frequency",frequency);
		shader.setFloat("_radius", fireRadius);
		
		
		//Draw shapes
		shader.setVec3("_ColorA", fireColor1);
		shader.setVec3("_ColorB", fireColor2);
		

		//TODO: Render point lights
		shader.setInt("numFlames", numFlames);

			for (int i = 0; i < numFlames; i++)
			{
				shader.setMat4("_Model", fireTransform[i].getModelMatrix());
				shader.setVec3("_Flames[" + std::to_string(i) + "].position", flames[i].position);
				shader.setVec3("_Flames[" + std::to_string(i) + "].color", flames[i].color);
				fireMesh.draw();
			}
		shader.setFloat("_Material.ambientK", material.ambientK);
		shader.setFloat("_Material.diffuseK", material.diffuseK);
		shader.setFloat("_Material.specular", material.specular);
		shader.setFloat("_Material.shininess", material.shininess);

		
		////Glenn Sakowicz**************************************
		for (float i = 0; i < gridSize; ++i)
			for (float j = 0; j < gridSize; ++j)
			{
				float x = i * stepSize;
				float y = j * stepSize;
				float z = perlin.noise(x, y) * 5.0f;
				int index = i * gridSize + j;
			}
		glDeleteVertexArrays(1, &terrainVAO);
		terrainVAO = createVAO(vertices.data(), vertices.size() * 3, indices.data(), indices.size());
		terrainShader.use();
		terrainShader.setMat4("projection", camera.ProjectionMatrix());
		terrainShader.setMat4("view", camera.ViewMatrix());

		glBindVertexArray(terrainVAO);
		glDrawElements(GL_TRIANGLES, terrain.getIndices().size(), GL_UNSIGNED_INT, 0);
		//*******************************************************************
		shader.setVec3("cameraPos", camera.position);
		//Render UI
		{
			//Owen Stanclift
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}

			}
				ImGui::SliderInt("NumFlames(0-4)", &numFlames,0,4);
				if (numFlames > 0)
				{
					for (int i = 0; i < numFlames; i++)
					{
						ImGui::PushID(i);
						{
							if (ImGui::CollapsingHeader("Flame"))
							{
								ImGui::DragFloat3("Position", &fireTransform[i].position.x, 0.1f);
								ImGui::DragFloat3("Scale", &fireTransform[i].scale.x, 0.1f);
							}
						}
						ImGui::PopID();
					}
					if (ImGui::CollapsingHeader("Flame Properties"))
					{
						ImGui::SliderFloat("Speed", &speed, 0, 20);
						ImGui::SliderFloat("Strength", &strength, 0, 3);
						ImGui::SliderFloat("Frequency", &frequency, 1, 5);
					}
					if (ImGui::CollapsingHeader("Material"))
					{
						ImGui::SliderFloat("Ambient", &material.ambientK, 0, 1);
						ImGui::SliderFloat("Diffuse", &material.diffuseK, 0, 1);
						ImGui::SliderFloat("Specular", &material.specular, 0, 1);
						ImGui::SliderFloat("Shininess", &material.shininess, 2, 10);
					}
				}



			ImGui::ColorEdit3("BG color", &bgColor.x);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	return vao;
}

