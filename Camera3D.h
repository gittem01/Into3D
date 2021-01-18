#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <math.h>

#define WALKER 1
#define SURROUNDER 2
#define THIRD_PERSON 3

class Camera3D
{
public:
	glm::vec3 pos;
	glm::vec3 rot;
	GLFWwindow* window;
	float zoom = 1;

	glm::vec2 zoomLimits = glm::vec2(0.5, 3);
	glm::mat4 pers;

	glm::vec2* lastMouse = NULL;

	glm::vec3 lookDir = glm::vec3(0, 0, 0);
	glm::vec3 topVec = glm::vec3(0, 1, 0);
	float dist = 20;
	glm::vec3 coordinateSystem[3] =
	{
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1)
	};

	void* atachedThing = NULL;

	int cameraType = WALKER;
	glm::vec2 dragAdd = glm::vec2(0, 0);
	int* mouseData;
	int* keyData;
	Camera3D(glm::vec3 pos, int* mouseData, int* keyData, GLFWwindow* window);
	void update();

	glm::mat4 getPers(int width, int height);
	glm::mat4 getView();
	void changeZoom(float inc);
	float limitZoom(float inZoom);
	void rotateFunc(int width, int height);
	void keyControl();
	void surrounderCamera(int diffx, int diffy);
	void walkerCamera(int diffx, int diffy);
	void cursorOutFunc(int width, int height);
	glm::vec3 rotatePoint(glm::vec3 point, glm::vec3 rotAngles);
	void updateLookDir();
	void updatePos();
	void updateTrdPos();
	void controlRotation();
	void atachBody(void* st);
	void detachBody();
};

