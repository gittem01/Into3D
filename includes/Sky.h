#pragma once

#include <math.h>
#include "Shader.h"
#include "Camera3D.h"
#include <vector>
#include <string.h>

struct Star
{
	glm::vec3 color;
	glm::vec2 rot;
	float brightness;
};

class Sky
{
public:
	std::vector<Star*> stars;
	Shader* starShader;
	unsigned int VAO;
	glm::vec3 rotationAxis;
	float currentSkyAngle;
	Sky();
	void update(Camera3D* cam);
	glm::mat4 getRotationMatrix();
	unsigned int getDefaultVAO();
};

