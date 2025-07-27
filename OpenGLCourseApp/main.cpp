#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "Shader.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"
#include "Model.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture("Textures/brick.png");
Texture dirtTexture("Textures/dirt.png");
Texture plainTexture("Textures/plain.png");

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformModel, uniformProjection, uniformView;

static const char* fShader = "Shaders/shader.fragment";
static const char* vShader = "Shaders/shader.vertex";

void calculateAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int indice0 = indices[i] * vLength;
		unsigned int indice1 = indices[i + 1] * vLength;
		unsigned int indice2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[indice1] - vertices[indice0], vertices[indice1 + 1] - vertices[indice0 + 1], vertices[indice1 + 2] - vertices[indice0 + 2]);
		glm::vec3 v2(vertices[indice2] - vertices[indice0], vertices[indice2 + 1] - vertices[indice0 + 1], vertices[indice2 + 2] - vertices[indice0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		indice0 += normalOffset;
		indice1 += normalOffset;
		indice2 += normalOffset;

		vertices[indice0] += normal.x;
		vertices[indice0 + 1] += normal.y;
		vertices[indice0 + 2] += normal.z;

		vertices[indice1] += normal.x;
		vertices[indice1 + 1] += normal.y;
		vertices[indice1 + 2] += normal.z;

		vertices[indice2] += normal.x;
		vertices[indice2 + 1] += normal.y;
		vertices[indice2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i += 3)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3,
	};

	//	  X		 Y	   Z		 U	   V		 NX    NY    NZ
	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* object1 = new Mesh();
	object1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(object1);

	Mesh* object2 = new Mesh();
	object2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(object2);

	Mesh* object3 = new Mesh();
	object3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(object3);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}


int main()
{
	// Create Window
	Window mainWindow = Window(1920, 1080);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	//Initialize Camera
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

	//Load Textures
	if (!brickTexture.LoadTexture()) {
		printf("Failed to load brick texture!\n");
	}
	if (!dirtTexture.LoadTexture()) {
		printf("Failed to load dirt texture!\n");
	}
	if (!plainTexture.LoadTexture()) {
		printf("Failed to load plain texture!\n");
	}

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj");

	// Initialize Lights & Count Variables
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								0.3f, 0.6f,
								0.0f, 0.0f, -1.0f);

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.1f, 0.1f,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);

	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.1f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.2f, 0.1f);

	pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							 0.0f, 1.0f,
						     0.0f, 0.0f, 0.0f,
							 0.0f, -1.0f, 0.0f,
							 0.3f, 0.2f, 0.1f,
							 20.0f);

	spotLightCount++;

	// Add Uniforms
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformCameraPosition = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;

	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	Assimp::Importer importer;


	// Loop Until Window Closed
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input Events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformCameraPosition = shaderList[0]->GetCameraPositionLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		spotLights[0].SetFlash(lowerLight, camera.GetCameraDirection());

		shaderList[0]->SetDirectionalLight(&mainLight);
		shaderList[0]->SetPointLights(pointLights, pointLightCount);
		shaderList[0]->SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformCameraPosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Pyramid 1
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[0]->RenderMesh();

		// Pyramid 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[1]->RenderMesh();

		// Floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		// X-Wing Model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		xwing.RenderModel();

		// Blackhawk Model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		blackhawk.RenderModel();

		// Unbind Shader Program
		glUseProgram(0);

		// Switch Buffers
		mainWindow.swapBuffers();
	}

	return 0;
}