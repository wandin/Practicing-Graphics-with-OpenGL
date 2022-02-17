// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
/* convert radians to degrees*/
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

/* true = right, false = left */
bool direction = true;
float triOffset = 0.0f;
float triIncrement = 0.0005f; /* increment 0.0005 until reach 0.7, then moves the opposite way reducing its value*/
float triMaxOffset = 0.7f; /* max value of movement, when reach 0.7 starts moving the opposite direction */

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = R"(
#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(pos, 1.0);
}
)";

// Fragment Shader
static const char* fShader = R"(
#version 330
            
out vec4 colour;       
       
void main()        
{
    colour = vec4(1.0, 0.0, 0.0, 1.0);                              
}
)";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    /* Create and bind vertex*/
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // bind VAO 

    /* Create and bind vertex buffer */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO


    glBindVertexArray(0); // unbind VAO 
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;
 
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);

		printf("Error compiling the %d shader:  '%s'\n", shaderType, eLog);
		return;
	}

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creating shader program");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER); // vertex shader
    AddShader(shader, fShader, GL_FRAGMENT_SHADER); // fragment shader

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);

        printf("Error linking program:  '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);

		printf("Error validating program:  '%s'\n", eLog);
		return;
	}

    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Text Window", NULL, NULL);
    if (!mainWindow)
    {
        // If window creation failed, terminate GLFW
        printf("GLFW Window creation failed!");
        glfwTerminate();

        return 1;
    }

    // Get Buffer size information

    // declare a buffer variable but doesn't initialize it, use getFramebuffersize to get it from our 'mainWindow'.
    int bufferWith, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWith, &bufferHeight);

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

    // Setup ViewPort size
    glViewport(0, 0, bufferWith, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + Handle user input events (mouse or keyboard inputs)
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction; /* invert the boolean */
        }

        curAngle += 0.01f;
        if (curAngle >= 360)
        {
            curAngle -= 360;
        }

        // it scales based on direction, scaling up when going right and down when going left
        if (direction)
        {

            curSize += 0.0001f;
        }
        else
        {
            curSize -= 0.0001f;
        }

        if (curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // grab and use the ID we creating, drawing the image.
        glUseProgram(shader);

        glm::mat4 model(1.0f); // create a glm matrix4 variable
        
        // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
        
        model = glm::scale(model, glm::vec3(curSize, 0.4f, 1.0f));

        

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
   
        

            glUseProgram(0); // clear the image

        glfwSwapBuffers(mainWindow);
    }
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
