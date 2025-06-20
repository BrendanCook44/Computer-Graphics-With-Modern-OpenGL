#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Constants
const float toRadians = 3.14159265359f / 180.0f;

// Function Prototypes
GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float currentAngle = 0.0f;

float sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "                         \n\
#version 330	                                  	   \n\
                                                       \n\
layout (location = 0) in vec3 pos;                     \n\
													   \n\
out vec4 vCol;					       				   \n\
													   \n\
uniform mat4 model;                                    \n\
													   \n\
													   \n\
void main()                                            \n\
{                                                      \n\
	gl_Position = model * vec4(pos, 1.0);		       \n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);   	   \n\
													   \n\
}";

// Fragment Shader
static const char* fShader = "                         \n\
#version 330	                                  	   \n\
													   \n\
in vec4 vCol;					       				   \n\
                                                       \n\
out vec4 color;					                       \n\
													   \n\
void main()                                            \n\
{                                                      \n\
	color = vCol;					      			   \n\
}";

// Function to Add Shaders
void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint shaderObject = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shaderObject, 1, code, codeLength);
	glCompileShader(shaderObject);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderObject, sizeof(eLog), NULL, eLog);
		printf("Error compiling shader type %d: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, shaderObject);
}

// Function to Compile Shaders
void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}


	uniformModel = glGetUniformLocation(shader, "model");
}

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 1.0f, // Bottom Left
		1.0f, -1.0f, 0.0f, // Bottom Right
		0.0f, 1.0f, 0.0f // Top Center
	};

	 // Generate and Bind Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and Bind Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy Vertices Array to Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable Vertex Attributes
	glEnableVertexAttribArray(0);

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);

}

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Window Properties
	// OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size Information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set Window Context for GLEW
	glfwMakeContextCurrent(mainWindow);

	// Allow Modern Extension Features
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Create Triangle
	CreateTriangle();

	// Compile Shaders
	CompileShaders();

	// Loop Until Window Closed
	while (!glfwWindowShouldClose(mainWindow))
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
		glClear(GL_COLOR_BUFFER_BIT);

		// Use Shader Program
		glUseProgram(shader);

		// Create Model Matrix
		glm::mat4 model(1.0f);

		// Apply Translation to Model Matrix
		model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Bind VAO and Draw
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Unbind VAO
		glBindVertexArray(0);

		// Unbind Shader Program
		glUseProgram(0);

		// Switch Buffers
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}