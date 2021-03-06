// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

/* convert radians to degrees*/
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

// creating vectors
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

/* creating camera*/
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
    //   X       Y     Z            U     V        - U V being the texture coordinates
        -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,        0.5f, 1.0f

    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
}


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(800, 600);
    mainWindow.initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 1.0f);

    // textures
    brickTexture = Texture((char*)("Images/brick.png"));
    brickTexture.LoadTexture();

    dirtTexture = Texture((char*)("Images/dirt.png"));
    dirtTexture.LoadTexture();

    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f); // a bit darker light

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        /* actual time to calculate delta time */
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // Get + Handle user input events (mouse or keyboard inputs)
        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // grab and use the ID we creating, drawing the image.
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformAmbientColor = shaderList[0].GetAmbientColorLocation(); // light color
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation(); // Ambient light intensity

        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor);


        // create  FIRST OBJECT
        glm::mat4 model(1.0f); 
        /* Translation, Rotation and Scaling */
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        /* Matrix for the camera, using the calculateViewMatrix function.*/
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        brickTexture.UseTexture(); // Using Textures, calling usetexture() function
        meshList[0]->RenderMesh();

        // create  SECOND OBJECT
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dirtTexture.UseTexture();  // Using Textures, calling usetexture() function
		meshList[1]->RenderMesh();

            // clear the image
            glUseProgram(0); 

        mainWindow.SwapBuffers();
    }
    return 0;
}
