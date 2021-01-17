#define _CRT_SECURE_NO_WARNINGS

#include <reactphysics3d/reactphysics3d.h>
#include <WindowPainter.h>
#include <Camera.h>
#include "Camera3D.h"
#include "Boxy.h"
#include <stdlib.h>
#include <iostream>

using namespace reactphysics3d;

int main()
{
	WindowPainter* p = new WindowPainter(0);
	Camera* cam = new Camera(glm::vec2(0, 0), p->mouseData, p->window);
	p->cam = cam;

	int n = 10;
	Boxy** boxes = (Boxy**)malloc(n * sizeof(Boxy*));

	Boxy* b = new Boxy(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	Boxy* b2 = new Boxy(glm::vec3(0, -5, 0), glm::vec3(100, 1, 100));

	Camera3D* cam3D = new Camera3D(glm::vec3(0, 0, 10), p->mouseData, p->window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);


	const float timeStep = 1.0f / 60.0f;
	PhysicsCommon* physicsCommon = new PhysicsCommon();
	PhysicsWorld* world = physicsCommon->createPhysicsWorld();

	world->setNbIterationsVelocitySolver(1);
	world->setNbIterationsPositionSolver(1);

	for (int i = 0; i < n; i++) {
		boxes[i] = new Boxy(glm::vec3(rand() % 12 - 6, rand() % 10 + 5, rand() % 12 - 6), 
			glm::vec3(rand()%3+1, rand() % 3 + 1, rand() % 3 + 1));
		boxes[i]->createRigidBody(BodyType::DYNAMIC, physicsCommon, world);
	}

	b->createRigidBody(BodyType::DYNAMIC, physicsCommon, world);
	b2->createRigidBody(BodyType::STATIC, physicsCommon, world);

	while ( !glfwWindowShouldClose( p->window ))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < n; i++) {
			boxes[i]->update(cam3D);
		}
		b->update(cam3D);
		b2->update(cam3D);
		p->looper();
		cam->update();
		cam3D->update();
		world->update(timeStep);
	}

	return 0;
}