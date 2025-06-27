#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
    public:
        Window();
        Window(GLint windowWidth, GLint windowHeight);
        ~Window();

        int Initialize();

        GLfloat getBufferWidth() { return bufferWidth; }
        GLfloat getBufferHeight() { return bufferHeight; }
        void swapBuffers() { glfwSwapBuffers(mainWindow); }
        bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    private:
        GLFWwindow* mainWindow;
        GLint width, height;
        GLint bufferWidth, bufferHeight;

        bool keys[1024];


};

