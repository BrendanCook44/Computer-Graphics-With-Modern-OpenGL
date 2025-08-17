#pragma once
#include "Light.h"


class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat shadowWidth, GLfloat shadowHeight);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::mat4 CalculateLightTransform();

private:
	glm::vec3 direction;
};

