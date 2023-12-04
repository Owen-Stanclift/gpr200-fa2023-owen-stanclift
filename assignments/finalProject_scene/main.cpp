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

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light
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

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/fireLit.vert", "assets/fireLit.frag");
	//ew::Shader lightShader("assets/unLit.vert", "assets/unLit.frag");
	unsigned int noiseTexture = ew::loadTexture("assets/noiseTexture.png", GL_REPEAT, GL_LINEAR);

	ew::Mesh fireMesh(myLib::createFire(0.5f, 60, 5));

	ew::Transform fireTransform;

	fireTransform.position = ew::Vec3(0, 1.0, 0);
	ew::Vec3 fireColor1 = ew::Vec3(1.0, 0.0, 0.0);
	ew::Vec3 fireColor2 = ew::Vec3(1.0, 1.0, 0.0);

	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();


		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*lightShader.use();
		lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());*/

		//for (int i = 0; i < numLights; i++)
		//{
		//	lightSphere[i].position = lights[i].position;
		//	lightShader.setMat4("_Model", lightSphere[i].getModelMatrix());
		//	lightShader.setVec3("_Color", lights[i].color);
		//	lightMesh.draw();
		//}


		shader.use();
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		

		//Draw shapes


		shader.setMat4("_Model", fireTransform.getModelMatrix());
		shader.setVec3("_ColorA", fireColor1);
		shader.setVec3("_ColorB", fireColor2);
		fireMesh.draw();

		//TODO: Render point lights
		//shader.setInt("numLights", numLights);

		//	for (int i = 0; i < numLights; i++)
		//	{
		//		
		//		shader.setVec3("_Lights[" + std::to_string(i) + "].position", lights[i].position);
		//		shader.setVec3("_Lights[" + std::to_string(i) + "].color", lights[i].color);
		//	}
		//shader.setFloat("_Material.ambientK", material.ambientK);
		//shader.setFloat("_Material.diffuseK", material.diffuseK);
		//shader.setFloat("_Material.specular", material.specular);
		//shader.setFloat("_Material.shininess", material.shininess);


		shader.setVec3("cameraPos", camera.position);
		//Render UI
		{
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
			/*	ImGui::SliderInt("NumLights(0-4)", &numLights,0,4);
				if (numLights > 0)
				{
					for (int i = 0; i < numLights; i++)
					{
						ImGui::PushID(i);
						{
							if (ImGui::CollapsingHeader("Light"))
							{
								ImGui::DragFloat3("Position", &lights[i].position.x, 0.1f);
								ImGui::DragFloat3("Color", &lights[i].color.x, 0.1f);
							}
						}
						ImGui::PopID();
					}
					if (ImGui::CollapsingHeader("Material"))
					{
						ImGui::SliderFloat("Ambient", &material.ambientK, 0, 1);
						ImGui::SliderFloat("Diffuse", &material.diffuseK, 0, 1);
						ImGui::SliderFloat("Specular", &material.specular, 0, 1);
						ImGui::SliderFloat("Shininess", &material.shininess, 2, 10);
					}
				}*/



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