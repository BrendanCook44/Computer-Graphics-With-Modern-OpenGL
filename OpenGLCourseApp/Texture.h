#pragma once
#include "stb_image.h"
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	bool LoadTexture();

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;

};

