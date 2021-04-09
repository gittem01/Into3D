#define _CRT_SECURE_NO_WARNINGS

#include "Camera3D.h"
#include "SmartThing.h"

Camera3D::Camera3D(glm::vec3 pos, WindowPainter* wp) {
	this->pos = pos;
	this->posAim = pos;

	this->rot = glm::vec3(0, 0, 0);
	this->rotAim = rot;

	this->wp = wp;
	this->window = wp->window;

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
		return glm::lookAt(this->pos, this->lookDir + this->pos, this->topVec);
	}
	else {
		return glm::lookAt(glm::vec3(0), this->lookDir, this->topVec);
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
	this->pers = getPers(width, height);

	if (this->wp->smoothMousePos[2] != 0)
	{
		this->changeZoom(0.1f * this->wp->smoothMousePos[2]);
	}

	if (!isAnimating){
		this->rotateFunc(width, height);
		this->updatePos();
	}
	else{
		glm::vec3 posDiff = (posAim - pos) * animSmoothness;
		glm::vec3 rotDiff = (rotAim - rot) * animSmoothness;
		pos += posDiff;
		rot += rotDiff;

		updateLookDir();

		if (glm::length(posDiff) < 0.001f && glm::length(rotDiff) < 0.001f){
			isAnimating = false;
		}
	}
}

void Camera3D::rotateFunc(int width, int height)
{
	if (abs(wp->smoothDiff[0]) > 0 || abs(wp->smoothDiff[1]) > 0)
	{
		this->rot.x += 0.01f * wp->smoothDiff[1] * wp->cameraSensitivity;
		this->rot.y += 0.01f * wp->smoothDiff[0] * wp->cameraSensitivity;
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
	this->lookDir = rotatePoint(lookDirection, this->rot);
}

void Camera3D::updatePos()
{
	if (cameraType == SURROUNDER) {
		glm::vec3 basePos;

		if (this->atachedThing == NULL) {
			basePos = glm::vec3(0, 0, this->dist);
			basePos = this->rotatePoint(basePos, this->rot);
		}
		else {
			SmartThing* s = (SmartThing*)this->atachedThing;
			controlRotation(&this->rot);	
			basePos = getSurroundPos(s, rot);
		}
		
		this->pos.x = basePos.x;
		this->pos.y = basePos.y;
		this->pos.z = basePos.z;
	}
	else if (cameraType == WALKER || cameraType == FIRST_PERSON) {
		updateFrstPos();
		controlRotation(&this->rot);
	}
	updateLookDir();
	keyControl();
}

void Camera3D::keyControl() {
	updateWlkrPos();
}

void Camera3D::updateWlkrPos()
{
	glm::vec3 cameraRight = glm::normalize(glm::cross(topVec, this->lookDir));
	float speedMult = wp->cameraSpeed;
	if (cameraType == WALKER) {
		glm::vec3 speedAddition = glm::vec3(0, 0, 0);

		if (wp->keyData[GLFW_KEY_LEFT_CONTROL]) {
			speedMult *= 5.0f;
		}
		if (wp->keyData[GLFW_KEY_LEFT_SHIFT]) {
			speedMult *= 0.2f;
		}

		if (wp->keyData[GLFW_KEY_W]) {
			speedAddition.x += freeSpeed * lookDir.x;
			speedAddition.y += freeSpeed * lookDir.y;
			speedAddition.z += freeSpeed * lookDir.z;
		}

		if (wp->keyData[GLFW_KEY_S]) {
			speedAddition.x -= freeSpeed * lookDir.x;
			speedAddition.y -= freeSpeed * lookDir.y;
			speedAddition.z -= freeSpeed * lookDir.z;
		}

		if (wp->keyData[GLFW_KEY_A]) {
			speedAddition.x += freeSpeed * cameraRight.x;
			speedAddition.y += freeSpeed * cameraRight.y;
			speedAddition.z += freeSpeed * cameraRight.z;
		}

		if (wp->keyData[GLFW_KEY_D]) {
			speedAddition.x -= freeSpeed * cameraRight.x;
			speedAddition.y -= freeSpeed * cameraRight.y;
			speedAddition.z -= freeSpeed * cameraRight.z;
		}

		if (wp->keyData[GLFW_KEY_Q]) {
			speedAddition.y += freeSpeed;
		}
		if (wp->keyData[GLFW_KEY_E]) {
			speedAddition.y -= freeSpeed;
		}

		posAim += speedAddition * speedMult;
		glm::vec3 diff = posAim * wp->keySmoothness;
		pos += diff;
		posAim -= diff;
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

void Camera3D::controlRotation(glm::vec3* rot) {
	if (cameraType == SURROUNDER) {
		if (rot->x > glm::radians(180.0f))
		{
			rot->x -= glm::radians(180.0f) * 2;
		}
		else if (rot->x < -glm::radians(180.0f))
		{
			rot->x += glm::radians(180.0f) * 2;
		}

		if (rot->x < glm::radians(0.0f)) {
			rot->x = glm::radians(0.0f);
		}
		else if (rot->x > glm::radians(90.0f)) {
			rot->x = glm::radians(90.0f) - 0.0001;
			topVec.y = -1; // Only for value radians(90)
		}

		if (abs(rot->x) >= glm::radians(90.0f))
		{
			topVec.y = -1;
		}
		else {
			topVec.y = +1;
		}
	}
	else {
		if (rot->x >= glm::radians(90.0f)) {
			rot->x = glm::pi<float>() / 2.001f;
		}
		else if (rot->x <= -glm::radians(90.0f)) {
			rot->x = -glm::pi<float>() / 2.001f;
		}
	}
}

void Camera3D::atachBody(void* st)
{
	this->atachedThing = st;
}

void Camera3D::detachBody()
{
	this->atachedThing = NULL;
	this->cameraType = WALKER;
}

glm::vec3 Camera3D::getSurroundPos(void* st, glm::vec3 angle){
	SmartThing* theSt = (SmartThing*)this->atachedThing;

	Vector3 p = theSt->rb->getTransform().getPosition();
	glm::vec3 basePos = glm::vec3(p.x, p.y, p.z);
	
	glm::vec3 lookPos = basePos + glm::vec3(0, 0, this->dist);
	lookPos = this->rotatePointArround(lookPos, basePos, angle);

	return lookPos;
}

glm::vec3 Camera3D::getVectorAngle(glm::vec3 vec){ // TODO
	vec = glm::normalize(vec);

	float angleY = asin(vec.x);
	float angleX = asin(vec.z)/cos(angleY);

	return glm::vec3(angleX, angleY, 0);
}

void Camera3D::switchCameraMode(CameraTypes type){ // TODO
	SmartThing* st = (SmartThing*)this->atachedThing;

	if (type != this->cameraType){
		if (type == SURROUNDER && false){ // for later use
			Vector3 p = st->rb->getTransform().getPosition();
			glm::vec3 v = glm::vec3(1, 10, 1);
			rotAim = getVectorAngle(v);

			posAim = getSurroundPos(st, rotAim);

			isAnimating = true;
		}

		cameraType = type;
	}	
}