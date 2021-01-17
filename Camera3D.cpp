#include "Camera3D.h"


Camera3D::Camera3D(glm::vec3 pos, int* mouseData, GLFWwindow* window) {
	this->pos = pos;
	this->rot = glm::vec3(0, 0, 0);
	this->mouseData = mouseData;
	this->window = window;

	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	this->pers = this->getPers(width, height);
}


glm::mat4 Camera3D::getPers(int width, int height) 
{
	return glm::perspective(glm::radians(45.0f/this->zoom), (float)width/(float)height, 0.1f, 100.0f);
}

glm::mat4 Camera3D::getView()
{
	return glm::lookAt(this->pos, this->lookPoint, this->topVec);
}

void Camera3D::changeZoom(float inc)
{
	this->zoom += inc;
	this->zoom = this->limitZoom(zoom);
}

float Camera3D::limitZoom(float inZoom) {
	if (inZoom < zoomLimits.x) {
		inZoom = zoomLimits.x;
	}
	else if (inZoom > zoomLimits.y) {
		inZoom = zoomLimits.y;
	}
	return inZoom;
}

void Camera3D::update() {
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);

	this->pers = this->getPers(width, height);

	if (this->mouseData[5] != 0)
	{
		this->changeZoom(0.1f * this->mouseData[5]);
	}
	this->rotateFunc(width, height);
	this->updatePos();

}

void Camera3D::rotateFunc(int width, int height)
{
	if (this->mouseData[3] && this->lastMouse)
	{
		int diffx = (mouseData[0] + dragAdd.x) - lastMouse->x;
		int diffy = (mouseData[1] + dragAdd.y) - lastMouse->y;
		if (cameraType == SURROUNDER) {
			surrounderCamera(diffx, diffy);
		}
		else if (cameraType == WALKER) {
			walkerCamera(diffx, diffy);
		}
	}
	if (!lastMouse && mouseData[3])
	{
		this->lastMouse = (glm::vec2*)malloc(1 * sizeof(glm::vec2));
	}
	if (!this->mouseData[3] && this->lastMouse)
	{
		free(this->lastMouse);
		this->lastMouse = NULL;
		this->dragAdd.x = 0; this->dragAdd.y = 0;
	}
	else if (lastMouse) {
		this->lastMouse->x = this->mouseData[0] + dragAdd.x;
		this->lastMouse->y = this->mouseData[1] + dragAdd.y;
		cursorOutFunc(width, height);
	}	
}

void Camera3D::surrounderCamera(int diffx, int diffy)
{
	this->rot.y += 0.01f * diffx;
	this->rot.x += 0.01f * diffy;
}

void Camera3D::walkerCamera(int diffx, int diffy)
{
	this->rot.y += 0.004f * diffx;
	this->rot.x += 0.004f * diffy;
}

void Camera3D::cursorOutFunc(int width, int height) {
	if (this->mouseData[0] > width - 2) {
		glfwSetCursorPos(window, 2, this->mouseData[1]);
		this->dragAdd.x += width;
	}
	else if (this->mouseData[0] < 1) {
		glfwSetCursorPos(window, width-2, this->mouseData[1]);
		this->dragAdd.x -= width;
	}

	if (this->mouseData[1] > height - 2) {
		glfwSetCursorPos(window, this->mouseData[0], 2);
		this->dragAdd.y += height;
	}
	else if (this->mouseData[1] < 1) {
		glfwSetCursorPos(window, this->mouseData[0], height-2);
		this->dragAdd.y -= height;
	}
}

glm::vec3 Camera3D::rotatePoint(glm::vec3 point, glm::vec3 rotAngles)
{
	glm::mat4 rotator = glm::mat4(1.0f);
	
	rotator = glm::rotate(rotator, rotAngles.x, glm::vec3(1, 0, 0));
	rotator = glm::rotate(rotator, rotAngles.y, glm::vec3(0, 1, 0));
	rotator = glm::rotate(rotator, rotAngles.z, glm::vec3(0, 0, 1));

	glm::vec4 v = glm::vec4(point.x, point.y, point.z, 1.0f) * rotator;

	return glm::vec3(v);
}

void Camera3D::updatePos()
{
	if (rot.x > glm::pi<float>())
	{
		rot.x -= glm::pi<float>() * 2;
	}
	else if (rot.x < -glm::pi<float>())
	{
		rot.x += glm::pi<float>() * 2;
	}

	if (abs(this->rot.x) > glm::radians(90.0f))
	{
		this->topVec.y = -1;
	}
	else {
		this->topVec.y = +1;
	}

	if (cameraType == SURROUNDER) {
		
		glm::vec3 basePos = glm::vec3(0, 0, this->dist);
		glm::vec3 baseTop = this->topVec;

		basePos = this->rotatePoint(basePos, this->rot);
		baseTop = this->rotatePoint(topVec, this->rot);

		this->pos.x = basePos.x;
		this->pos.y = basePos.y;
		this->pos.z = basePos.z;
	}
	else if (cameraType == WALKER) {
		glm::vec3 lookDirection = glm::vec3(0, 0, -1);

		this->lookPoint = glm::normalize(rotatePoint(lookDirection, this->rot)) + this->pos;
	}
}