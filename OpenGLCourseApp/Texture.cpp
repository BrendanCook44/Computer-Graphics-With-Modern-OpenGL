#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture(const char* fileLocation)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->fileLocation = fileLocation;

}

void Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!textureData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format = GL_RGB;
	if (bitDepth == 1)
		format = GL_RED;
	else if (bitDepth == 3)
		format = GL_RGB;
	else if (bitDepth == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}


Texture::~Texture()
{
	ClearTexture();
}