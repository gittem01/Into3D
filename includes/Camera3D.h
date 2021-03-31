#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <math.h>
#include "WindowPainter.h"

typedef enum CameraTypes {
	WALKER = 1,
	SURROUNDER = 2,
	FIRST_PERSON = 3,
}CameraTypes;

class Camera3D
{
public:
	glm::vec3 pos;
	glm::vec3 posAim;

	glm::vec3 rot;
	glm::vec3 rotAim;

	bool isAnimating = false; // Will be inished later
	float animSmoothness = 0.05f;

	GLFWwindow* window;
	float zoom = 1;
	float freeSpeed = 0.1f;

	int w, h; // Screen Sizes
	WindowPainter* wp;


	glm::vec2 zoomLimits = glm::vec2(0.5, 3);
	glm::mat4 pers;

	glm::vec3 lookDir = glm::vec3(0, 0, -1);
	glm::vec3 topVec = glm::vec3(0, 1, 0);
	float dist = 10;
	
	glm::vec3 coordinateSystem[3] =
	{
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1)
	};

	void* atachedThing = NULL;

	CameraTypes cameraType = WALKER;
	glm::vec2 dragAdd = glm::vec2(0, 0);

	Camera3D(glm::vec3 pos, WindowPainter* wp);
	void update();

	glm::mat4 getPers(int width, int height);
	glm::mat4 getView(bool posIncl);
	void changeZoom(float inc);
	float limitZoom(float inZoom);
	void rotateFunc(int width, int height);
	void keyControl();
	void surrounderCamera(float diffx, float diffy);
	void walkerCamera(float diffx, float diffy);
	void cursorOutFunc(int width, int height);
	glm::vec3 rotatePoint(glm::vec3 point, glm::vec3 rotAngles);
	glm::vec3 rotatePointArround(glm::vec3 point, glm::vec3 arroundPoint, glm::vec3 rotAngles);
	void updateLookDir();
	void updatePos();
	void updateWlkrPos();
	void updateFrstPos();
	void controlRotation(glm::vec3* rot);
	void atachBody(void* st);
	void detachBody();
	glm::vec3 getSurroundPos(void* st, glm::vec3 angle);
	glm::vec3 getVectorAngle(glm::vec3 vec);
	void switchCameraMode(CameraTypes type);
};

