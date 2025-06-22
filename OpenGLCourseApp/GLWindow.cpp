#include "GLWindow.h"

GLWindow::GLWindow()
{
    width = 800;
    height = 600;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
}

int GLWindow::Initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed!\n");
        glfwTerminate();
        return 1;
    }

    // Set GLFW Window Properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create GLFW Window
    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }
    
    // Get Buffer Size Information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set Context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed!\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }    // Set Viewport Size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    return 0;
}

GLWindow::~GLWindow()
{
    // Destroy GLFW Window
    if (mainWindow)
    {
        glfwDestroyWindow(mainWindow);
    }

    // Terminate GLFW
    glfwTerminate();
}