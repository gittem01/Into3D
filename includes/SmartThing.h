#pragma once

#include "Boxy.h"
#define GROUNDED 1
#define ONAIR 2

class SmartThing: public Boxy
{
public:
	static SmartThing* activatedThing;
	Vector3* lastSpeed = NULL;
	Vector3 acc = Vector3(0, 0, 0);
	int smartStatus = ONAIR;
	float speed = 10;
	float jumpSpeed = 5;
	float camHeight = 0.8;
	SmartThing(glm::vec3 pos, glm::vec3 size);
	void update(Camera3D* cam);
	void keyControl(Camera3D* cam);
	void jumpControl();
};
