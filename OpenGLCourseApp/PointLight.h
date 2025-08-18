#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

class PointLight : public Light
{

public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	glm::vec3 GetPosition();
	GLfloat GetFarPlane();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};

