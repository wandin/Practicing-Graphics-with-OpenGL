#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	// We're inheriting from Light Class, that handles all those variables in its constructor.
	// So here as it's a DIRECTIONAL Light, we will mostly take care of the Direction itself.
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) :	Light(red, green, blue, aIntensity, dIntensity)
{
	// Same Logic as in the basic constructor, inheriting most variables from Light class!

	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}
