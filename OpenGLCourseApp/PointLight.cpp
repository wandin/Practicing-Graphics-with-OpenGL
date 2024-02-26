#include "PointLight.h"

PointLight::PointLight()
{
	position = glm::vec3(0,0,0);
	constant = 1.f; // just setting to one to make sure our quadrantic equation has some 1 value, to avoid division by 0
	linear = 0.f;
	exponent = 0.f;

}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation,
							GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform3f(positionLocation, position.x, position.y, position.z);

	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

}

PointLight::~PointLight()
{

}
