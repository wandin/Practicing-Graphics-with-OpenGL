#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = false;

	for (size_t i = 0; i < 1024; i++)
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

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}


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

	// Create keys + mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	/* closes the window if ESC is pressed*/
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}