#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
	Shader();

	std::string ReadFile(const char* fileLocation);
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	GLuint GetModelLocation();
	GLuint GetProjectionLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetUniformDirection();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetCameraPositionLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();

private:

	int pointLightCount;

	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformProjection;
	GLuint uniformView;
	GLuint uniformEyePosition;

	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformCameraPosition;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	void CompileShader(const char* vertexCode, const char* fragmentCode);

};

