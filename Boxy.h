#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include "Shader.h"
#include "Camera3D.h"
#include <vector>
#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;

class Boxy
{
public:
	unsigned int VAO;
	unsigned int TEX;
	unsigned int id;
	RigidBody* rb;
	Shader* shader;
	glm::vec3* pos;
	glm::vec3* rotation;
	glm::vec3* size;
	glm::vec4* color;
	Boxy(glm::vec3 pos, glm::vec3 size);
	void update(Camera3D* cam);
	void createRigidBody(BodyType bType, PhysicsCommon* pc, PhysicsWorld* world);
	glm::mat4 getModel();
	glm::mat4 getRotation();
	unsigned int getDefaultVAO();
};