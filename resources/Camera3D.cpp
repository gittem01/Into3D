#define _CRT_SECURE_NO_WARNINGS

#include "Camera3D.h"
#include "SmartThing.h"

Camera3D::Camera3D(glm::vec3 pos, int* mouseData, int* keyData, GLFWwindow* window) {
	this->pos = pos;
	this->rot = glm::vec3(0, 0, 0);
	this->mouseData = mouseData;
	this->keyData = keyData;
	this->window = window;

	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	this->pers = this->getPers(width, height);
	this->w = width; this->h = height;
}


glm::mat4 Camera3D::getPers(int width, int height)
{
	return glm::perspective(glm::radians(45.0f / this->zoom), (float)width / (float)height, 0.1f, 500.0f);
}

glm::mat4 Camera3D::getView(bool posIncl)
{
	if (posIncl) {
		if (this->cameraType == WALKER || this->cameraType == FIRST_PERSON) {
			return glm::lookAt(this->pos, this->lookDir + this->pos, this->topVec);
		}
		else if (this->cameraType == SURROUNDER) {
			return glm::lookAt(this->pos, this->lookPos, this->topVec);
		}
	}
	else {
		if (this->cameraType == WALKER || this->cameraType == FIRST_PERSON) {
			return glm::lookAt(glm::vec3(0), this->lookDir, this->topVec);
		}
		else if (this->cameraType == SURROUNDER) {
			return glm::lookAt(glm::vec3(0), this->lookPos - this->pos, this->topVec);
		}
	}
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
	this->w = width; this->h = height;
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
		int diffx, diffy;

		if (cameraType == SURROUNDER) {
			diffx = (mouseData[0] + dragAdd.x) - lastMouse->x;
			diffy = -(mouseData[1] + dragAdd.y) + lastMouse->y;
			surrounderCamera(diffx, diffy);
		}
		else if (cameraType == WALKER || cameraType == FIRST_PERSON) {
			diffx = (mouseData[0] + dragAdd.x) - lastMouse->x;
			diffy = (mouseData[1] + dragAdd.y) - lastMouse->y;
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
	this->rot.x -= 0.01f * diffy;
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
		glfwSetCursorPos(window, width - 2, this->mouseData[1]);
		this->dragAdd.x -= width;
	}

	if (this->mouseData[1] > height - 2) {
		glfwSetCursorPos(window, this->mouseData[0], 2);
		this->dragAdd.y += height;
	}
	else if (this->mouseData[1] < 1) {
		glfwSetCursorPos(window, this->mouseData[0], height - 2);
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

glm::vec3 Camera3D::rotatePointArround(glm::vec3 point, glm::vec3 arroundPoint, glm::vec3 rotAngles) {
	glm::vec3 diff = point - arroundPoint;
	glm::mat4 rotator = glm::mat4(1.0f);

	rotator = glm::rotate(rotator, rotAngles.x, glm::vec3(1, 0, 0));
	rotator = glm::rotate(rotator, rotAngles.y, glm::vec3(0, 1, 0));
	rotator = glm::rotate(rotator, rotAngles.z, glm::vec3(0, 0, 1));

	glm::vec4 v = glm::vec4(diff.x, diff.y, diff.z, 1.0f) * rotator;

	return glm::vec3(v) + arroundPoint;
}

void Camera3D::updateLookDir() {
	glm::vec3 lookDirection = glm::vec3(0, 0, -1);
	this->lookDir = glm::normalize(rotatePoint(lookDirection, this->rot));
}
void Camera3D::updatePos()
{
	if (cameraType == SURROUNDER) {
		glm::vec3 basePos;
		glm::vec3 baseTop = this->topVec;

		if (this->atachedThing == NULL) {
			basePos = glm::vec3(0, 0, this->dist);
			basePos = this->rotatePoint(basePos, this->rot);
		}
		else {
			SmartThing* s = (SmartThing*)this->atachedThing;
			Vector3 p = s->rb->getTransform().getPosition();
			lookPos.x = p.x;
			lookPos.y = p.y;
			lookPos.z = p.z;
			if (this->rot.x < glm::radians(0.001f)) {
				this->rot.x = glm::radians(0.001f);
			}
			else if (this->rot.x > glm::radians(89.99f)) {
				this->rot.x = glm::radians(89.99f);
			}
			basePos = lookPos + glm::vec3(0, 0, this->dist);
			basePos = this->rotatePointArround(basePos, lookPos, this->rot);
			this->lookDir = glm::normalize(lookPos - basePos);
		}

		baseTop = this->rotatePoint(topVec, this->rot);

		this->pos.x = basePos.x;
		this->pos.y = basePos.y;
		this->pos.z = basePos.z;
		controlRotation();
	}
	else if (cameraType == WALKER || cameraType == FIRST_PERSON) {
		updateFrstPos();
		controlRotation();
		updateLookDir();
	}

	keyControl();
}

void Camera3D::keyControl() {
	updateWlkrPos();
}

void Camera3D::updateWlkrPos()
{
	glm::vec3 cameraRight = glm::normalize(glm::cross(topVec, this->lookDir));
	float speedMult = 1;
	if (cameraType == WALKER) {
		Vector3 speedAddition = Vector3(0, 0, 0);

		if (keyData[GLFW_KEY_LEFT_CONTROL]) {
			speedMult = 5.0f;
		}
		if (keyData[GLFW_KEY_LEFT_SHIFT]) {
			speedMult = 0.2f;
		}

		if (keyData[GLFW_KEY_W]) {
			speedAddition.x += freeSpeed * lookDir.x;
			speedAddition.y += freeSpeed * lookDir.y;
			speedAddition.z += freeSpeed * lookDir.z;
		}

		if (keyData[GLFW_KEY_S]) {
			speedAddition.x -= freeSpeed * lookDir.x;
			speedAddition.y -= freeSpeed * lookDir.y;
			speedAddition.z -= freeSpeed * lookDir.z;
		}

		if (keyData[GLFW_KEY_A]) {
			speedAddition.x += freeSpeed * cameraRight.x;
			speedAddition.y += freeSpeed * cameraRight.y;
			speedAddition.z += freeSpeed * cameraRight.z;
		}

		if (keyData[GLFW_KEY_D]) {
			speedAddition.x -= freeSpeed * cameraRight.x;
			speedAddition.y -= freeSpeed * cameraRight.y;
			speedAddition.z -= freeSpeed * cameraRight.z;
		}

		if (keyData[GLFW_KEY_Q]) {
			speedAddition.y += freeSpeed;
		}
		if (keyData[GLFW_KEY_E]) {
			speedAddition.y -= freeSpeed;
		}


		this->pos.x = pos.x + speedAddition.x * speedMult;
		this->pos.y = pos.y + speedAddition.y * speedMult;
		this->pos.z = pos.z + speedAddition.z * speedMult;
	}
}

void Camera3D::updateFrstPos()
{
	if (cameraType == FIRST_PERSON) {
		SmartThing* s = (SmartThing*)this->atachedThing;
		Vector3 pos = s->rb->getWorldPoint(Vector3(0, s->camHeight * (s->size->y / 2), 0));
		this->pos.x = pos.x;
		this->pos.y = pos.y;
		this->pos.z = pos.z;
	}
	else {
		return;
	}
}

void Camera3D::controlRotation() {
	if (cameraType == SURROUNDER) {
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
	}
	else {
		if (rot.x >= glm::pi<float>() / 2.0f) {
			rot.x = glm::pi<float>() / 2.001f;
		}
		else if (rot.x <= -glm::pi<float>() / 2.0f) {
			rot.x = -glm::pi<float>() / 2.001f;
		}
	}

}

void Camera3D::atachBody(void* st)
{
	this->atachedThing = st;
	this->cameraType = FIRST_PERSON;
}

void Camera3D::detachBody()
{
	this->atachedThing = NULL;
	this->cameraType = WALKER;
}