#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

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

        void UseShader();
        void ClearShader();

        ~Shader();

    private:
        GLuint shaderID;
        GLuint uniformModel;
        GLuint uniformProjection;
        GLuint uniformView;
        GLuint uniformEyePosition;

        GLuint uniformAmbientColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;

        GLuint uniformSpecularIntensity;
        GLuint uniformShininess;
        GLuint uniformCameraPosition;

        void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
        void CompileShader(const char* vertexCode, const char* fragmentCode);

};

