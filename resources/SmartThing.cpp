#include "SmartThing.h"

SmartThing* SmartThing::activatedThing = NULL;

SmartThing::SmartThing(glm::vec3 pos, glm::vec3 size): Boxy(pos, size) {
	this->color->y = 0; this->color->z = 0;
}


void SmartThing::update(Camera3D* cam) {
	Collider* c = rb->getCollider(0);
	Material& m = c->getMaterial();
	m.setMassDensity(1);
	this->jumpControl();
	this->keyControl(cam);
	Transform t = this->rb->getTransform();
	Vector3 v = t.getPosition();
	this->pos->x = v.x;
	this->pos->y = v.y;
	this->pos->z = v.z;
	this->rb->setAngularVelocity(Vector3(0, 0, 0));
	t.setOrientation(Quaternion::identity());
	rb->setTransform(t);


	if (cam->atachedThing != this || cam->cameraType != FIRST_PERSON) {
		this->shader->use();
		glm::mat4 pers = cam->pers;
		glm::mat4 view = cam->getView(true);
		glm::mat4 model = this->getModel();
		glm::mat4 rot = this->getRotation();

		this->shader->setMat4("pers", pers);
		this->shader->setMat4("view", view);
		this->shader->setMat4("model", model);
		this->shader->setMat4("rot", rot);

		this->shader->setVec3("lightPos", cam->pos.x, cam->pos.y, cam->pos.z);
		this->shader->setVec3("lightColor", 1, 1, 1);
		this->shader->setVec3("objColor", color->x, color->y, color->z);

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

void SmartThing::keyControl(Camera3D* cam) 
{
	glm::vec2 dirVec = glm::vec2(cam->lookDir.x, cam->lookDir.z);
	glm::vec3 p = cam->rotatePoint(glm::vec3(dirVec.x, 0, dirVec.y), glm::vec3(0, PI/2, 0));
	glm::vec2 dirVecTan = glm::vec2(p.x, p.z);

	glm::vec3 finalSpeed = glm::vec3(0);

	if (SmartThing::activatedThing == this && cam->atachedThing == this) 
	{
		if (cam->wp->keyData[GLFW_KEY_W])
		{
			finalSpeed.x += speed * dirVec.x;
			finalSpeed.z += speed * dirVec.y;
		}

		if (cam->wp->keyData[GLFW_KEY_S])
		{
			finalSpeed.x -= speed * dirVec.x;
			finalSpeed.z -= speed * dirVec.y;
		}

		if (cam->wp->keyData[GLFW_KEY_A])
		{
			finalSpeed.x -= speed * dirVecTan.x;
			finalSpeed.z -= speed * dirVecTan.y;
		}

		if (cam->wp->keyData[GLFW_KEY_D])
		{
			finalSpeed.x += speed * dirVecTan.x;
			finalSpeed.z += speed * dirVecTan.y;
		}
		if (cam->wp->keyData[GLFW_KEY_SPACE] && smartStatus == GROUNDED) {
			finalSpeed.y = jumpSpeed;
		}
		Vector3 vel = this->rb->getLinearVelocity();
		this->rb->setLinearVelocity(Vector3(finalSpeed.x, vel.y+finalSpeed.y, finalSpeed.z));
	}
	else
	{
		return;
	}
}

void SmartThing::jumpControl() {
	Vector3 spd = rb->getLinearVelocity();
	if (lastSpeed) {
		acc = spd - *lastSpeed;
	}
	else {
		this->lastSpeed = (Vector3*)malloc(1 * sizeof(Vector3));
	}
	this->lastSpeed->x = spd.x;
	this->lastSpeed->y = spd.y;
	this->lastSpeed->z = spd.z;
	
	if (abs(acc.y) <= 0.01) {
		smartStatus = GROUNDED;
	}
	else {
		smartStatus = ONAIR;
	}
}