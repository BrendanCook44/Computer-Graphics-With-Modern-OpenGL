#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformDirectionalLight.uniformColor = 0;
	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformCameraPosition = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.\n", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformDirectionalLight.uniformColor = 0;
	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformCameraPosition = 0;

	pointLightCount = 0;
	spotLightCount = 0;

	CompileShader(vertexCode, fragmentCode);

}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}


	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locationBuffer);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locationBuffer);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPosition = glGetUniformLocation(shaderID, "lightPosition");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		char locationBuffer[100] = { '\0' };

		snprintf(locationBuffer, sizeof(locationBuffer), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locationBuffer);
	}
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetUniformDirection()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetCameraPositionLocation()
{
	return uniformCameraPosition;
}

GLuint Shader::GetOmniLightPositionLocation()
{
	return uniformOmniLightPosition;
}

GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
	{
		lightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* directionalLightTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*directionalLightTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformDirectionalLight.uniformColor = 0;
	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformCameraPosition = 0;

}

Shader::~Shader()
{
	ClearShader();
}