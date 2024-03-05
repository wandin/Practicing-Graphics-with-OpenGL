#pragma once

#include <vector>

#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light
{

public:

	PointLight();

	PointLight( GLuint shadowWidth, GLuint shadowHeight,
				GLfloat near, GLfloat far,
				GLfloat red, GLfloat green, GLfloat blue,
				GLfloat aIntensity, GLfloat dIntensity,
				GLfloat xPos, GLfloat yPos, GLfloat zPos,
				GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
					GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

	~PointLight();

protected:

	glm::vec3 position; // where the light comes from

	/* values to be used in our quadrantic equation  (ax^2 + bx + c)    */
	GLfloat exponent;	// a
	GLfloat linear;		// b
	GLfloat constant;	// c

	GLfloat farplane;


};

