#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::initialise()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	/*  Setup GLFW window properties*/
	/*  Open GL version - in this case we are setting to version 3.3 = (MAJOR).(MINOR) */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Practicing OpenGL with C++", NULL, NULL);
	if (!mainWindow)
	{
		// If window creation failed, terminate GLFW
		printf("GLFW Window creation failed!");
		glfwTerminate();

		return 1;
	}

	// Get Buffer size information

	// declare a buffer variable but doesn't initialize it, use getFramebuffersize to get it from our 'mainWindow'.
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension features

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		// If initialization of GLEW fails, destroy window
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	/* enabling depth testing to be able to check which face of the triangle should be drawn in front of others*/
	glEnable(GL_DEPTH_TEST);

	// Setup ViewPort size
	glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
