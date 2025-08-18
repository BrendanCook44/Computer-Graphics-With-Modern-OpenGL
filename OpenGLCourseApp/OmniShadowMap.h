#pragma once
#include "ShadowMap.h"
class OmniShadowMap : public ShadowMap
{

public:
	bool Init(GLuint width, GLuint height);

	void Write();

	void Read(GLenum textureUnit);


private:


};

