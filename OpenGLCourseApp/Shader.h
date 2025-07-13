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

        void UseShader();
        void ClearShader();

        ~Shader();

    private:
        GLuint shaderID, uniformModel, uniformProjection, uniformView;

        void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
        void CompileShader(const char* vertexCode, const char* fragmentCode);

};

