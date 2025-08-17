#pragma once

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* directionalLightTransform);

	void UseShader();
	void ClearShader();

	~Shader();

private:

	int pointLightCount;
	int spotLightCount;

	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformProjection;
	GLuint uniformView;

	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformTexture;

	GLuint uniformCameraPosition;

	GLuint uniformDirectionalLightTransform;
	GLuint uniformDirectionalShadowMap;

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

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	void CompileShader(const char* vertexCode, const char* fragmentCode);

};

