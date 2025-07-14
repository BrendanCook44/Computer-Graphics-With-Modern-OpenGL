#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;
    xChange = 0.0f;
    yChange = 0.0f;

    for(size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    xChange = 0.0f;
    yChange = 0.0f;

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

    // Handle Key + Mouse Input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
    GLfloat changeValue = xChange;
    xChange = 0.0f;

    return changeValue;
}

GLfloat Window::getYChange()
{
    GLfloat changeValue = yChange;
    yChange = 0.0f;

    return changeValue;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* mainWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            mainWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            mainWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPosition, double yPosition)
{
    Window* mainWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (mainWindow->mouseFirstMoved)
    {
        mainWindow->lastX = xPosition;
        mainWindow->lastY = yPosition;
        mainWindow->mouseFirstMoved = false;
    }

    mainWindow->xChange = xPosition - mainWindow->lastX;
    mainWindow->yChange = mainWindow->lastY - yPosition;

    mainWindow->lastX = xPosition;
    mainWindow->lastY = yPosition;

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