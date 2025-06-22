#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
    public:
        GLWindow();
        GLWindow(GLint windowWidth, GLint windowHeight);
        ~GLWindow();

        int Initialize();

        GLfloat getBufferWidth() { return bufferWidth; }
        GLfloat getBufferHeight() { return bufferHeight; }
        void swapBuffers() { glfwSwapBuffers(mainWindow); }
        bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    private:
        GLFWwindow* mainWindow;
        GLint width, height;
        GLint bufferWidth, bufferHeight;
};

