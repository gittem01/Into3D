#define _CRT_SECURE_NO_WARNINGS

#include "WindowPainter.h"
#include "SmartThing.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "Sky.h"

using namespace reactphysics3d;

const Vector3 gravity = Vector3(0, -9.81f, 0);

int main()
{
	srand(time(0));
	WindowPainter* p = new WindowPainter();

	Sky* sky = new Sky();

	std::string st1 = "/defaultShaders/";
	std::string st2 = BASE_DIR;
	std::string final = st2 + st1;
	Boxy::shader = new Shader(final);

	int n = 20;
	Boxy** boxes = (Boxy**)malloc(n * sizeof(Boxy*));

	Boxy* b2 = new Boxy(glm::vec3(0, -5, 0), glm::vec3(100, 1, 100));

	Camera3D* cam3D = new Camera3D(glm::vec3(0, 0, 10), p);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	const float timeStep = 1.0f / 60.0f;
	PhysicsCommon* physicsCommon = new PhysicsCommon();
	PhysicsWorld* world = physicsCommon->createPhysicsWorld();

	world->setNbIterationsVelocitySolver(10);
	world->setNbIterationsPositionSolver(10);

	for (int i = 0; i < n; i++) {
		boxes[i] = new Boxy(glm::vec3(rand() % 12 - 6, rand() % 10 + 5, rand() % 12 - 6), 
			glm::vec3(rand()%3+1, rand() % 3 + 1, rand() % 3 + 1));
		boxes[i]->createRigidBody(BodyType::DYNAMIC, physicsCommon, world);
	}

	b2->createRigidBody(BodyType::STATIC, physicsCommon, world);

	SmartThing* st = new SmartThing(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	st->createRigidBody(BodyType::DYNAMIC, physicsCommon, world);
	SmartThing::activatedThing = st;

	cam3D->cameraType = static_cast<CameraTypes>(p->current_pos + 1);

	while (!glfwWindowShouldClose( p->window ))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Boxy::shader->use();
		for (int i = 0; i < n; i++) {
			boxes[i]->update(cam3D);
		}
		b2->update(cam3D);
		st->update(cam3D);
		sky->update(cam3D);

		if (p->enableGravity) {
			world->setGravity(gravity);
		}
		else {
			world->setGravity(Vector3(0, 0, 0));
		}
		
		world->update(timeStep);

		if (p->current_pos > 0) {
			cam3D->atachBody(st);
			cam3D->switchCameraMode(static_cast<CameraTypes>(p->current_pos + 1));
		}
		else {
			cam3D->detachBody();
			cam3D->switchCameraMode(CameraTypes::WALKER);
		}
		cam3D->update();
		p->looper();
	}

	return 0;
}