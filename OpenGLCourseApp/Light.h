#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		  GLfloat shadowWidth, GLfloat shadowHeight);

	ShadowMap* GetShadowMap()
	{
		return shadowMap;
	}

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProjection;

	ShadowMap* shadowMap;
};

