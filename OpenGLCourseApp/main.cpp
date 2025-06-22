#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "GLWindow.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLuint uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float currentAngle = 0.0f;

float sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

static const char* fShader = "Shaders/shader.fragment";
static const char* vShader = "Shaders/shader.vertex";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	Mesh *object1 = new Mesh();
	object1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(object1);

	Mesh *object2 = new Mesh();
	object2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(object2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}


int main()
{
	// Create Window
	GLWindow mainWindow = GLWindow(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();	
	
	// Add Projection Matrix
	glm::mat4 projection(1.0f);
	projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop Until Window Closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle User Input Events
		glfwPollEvents();

		// Update Triangle Offset
		// Change Direction if Offset Exceeds Max Offset
		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		// Update Current Angle
		// Increment by a Small Value and Wrap Around at 360 Degrees
		currentAngle += 0.1f;
		if (currentAngle >= 360.0f)
		{
			currentAngle -= 360.0f;
		}

		// Update Triangle Size
		// Change Direction if Size Exceeds Max Size
		if (sizeDirection)
		{
			currentSize += 0.001f;
		}
		else
		{
			currentSize -= 0.001f;
		}

		if (currentSize >= maxSize || currentSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		// Create Model Matrix
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Unbind Shader Program
		glUseProgram(0);

		// Switch Buffers
		mainWindow.swapBuffers();
	}

	return 0;
}