#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{

public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edge,
		GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);

	void Toggle() { isOn = !isOn; }

	void SetFlash(glm::vec3 pos, glm::vec3 dir);

private:
	glm::vec3 direction;

	GLfloat edge;
	GLfloat processEdge;

	bool isOn;
};

