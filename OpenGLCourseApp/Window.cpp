#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;

    for(size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    for(size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int Window::Initialize()
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

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed!\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Create Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
    
    return 0;
}

void handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* mainWindow = static_cast
}

Window::~Window()
{
    // Destroy GLFW Window
    if (mainWindow)
    {
        glfwDestroyWindow(mainWindow);
    }

    // Terminate GLFW
    glfwTerminate();
}