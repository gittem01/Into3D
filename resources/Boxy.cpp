#include "Boxy.h"

Boxy::Boxy(glm::vec3 pos, glm::vec3 size) {

	this->VAO = this->getDefaultVAO();
	std::string st1 = "/defaultShaders/";
	std::string st2 = BASE_DIR;
	std::string final = st2 + st1;
	this->shader = new Shader(final.c_str());
	this->pos = new glm::vec3(pos.x, pos.y, pos.z);
	this->rotation = new glm::vec3(0, 0, 0);
	this->size = new glm::vec3(size.x, size.y, size.z);
	this->color = new glm::vec4(1, 1, 1, 1);
}

void Boxy::update(Camera3D* cam) {
	Transform t = this->rb->getTransform();
	Vector3 v = t.getPosition();
	this->pos->x = v.x;
	this->pos->y = v.y;
	this->pos->z = v.z;

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

void Boxy::createRigidBody(BodyType bType, PhysicsCommon* pc, PhysicsWorld* world)
{
	BoxShape* boxShape = pc->createBoxShape(Vector3(size->x/2, size->y/2, size->z/2));
	Quaternion orientation = Quaternion::fromEulerAngles(Vector3(rotation->x, rotation->y, rotation->z));
	Transform transform = Transform(Vector3(pos->x, pos->y, pos->z), orientation);
	
	this->rb = world->createRigidBody(transform);
	rb->addCollider(boxShape, Transform::identity());
	rb->setType(bType);
	Collider* c = rb->getCollider(0);
	Material& m = c->getMaterial();
	m.setBounciness(0);
	m.setFrictionCoefficient(1);
	m.setMassDensity(100);
}

glm::mat4 Boxy::getModel() {

	glm::mat4 model = glm::mat4(1);
	glm::mat4 rot = this->getRotation();

	model = glm::translate(model, glm::vec3(pos->x, pos->y, pos->z));
	model *= rot;
	model = glm::scale(model, glm::vec3(size->x, size->y, size->z));
	return model;
}

glm::mat4 Boxy::getRotation() {
	Transform t = this->rb->getTransform();
	Quaternion q = t.getOrientation();
	return glm::quat(q.w, q.x, q.y, q.z).operator glm::mat<4, 4, glm::f32, glm::packed_highp>();
}

unsigned int Boxy::getDefaultVAO() {
	float viis[] = {
		// Vertex positions, Tex coords, Normal vectors
		-0.5f, +0.5f, +0.5f, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, // Front
		+0.5f, +0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 0.0f, +1.0f,
		+0.5f, -0.5f, +0.5f, 1.0f, 1.0f, 0.0f, 0.0f, +1.0f,
		-0.5f, -0.5f, +0.5f, 0.0f, 1.0f, 0.0f, 0.0f, +1.0f,

		-0.5f, +0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Back
		+0.5f, +0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		+0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, +0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //Left
		-0.5f, +0.5f, +0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, +0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

		+0.5f, +0.5f, -0.5f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, // Right
		+0.5f, +0.5f, +0.5f, 1.0f, 0.0f, +1.0f, 0.0f, 0.0f,
		+0.5f, -0.5f, +0.5f, 1.0f, 1.0f, +1.0f, 0.0f, 0.0f,
		+0.5f, -0.5f, -0.5f, 0.0f, 1.0f, +1.0f, 0.0f, 0.0f,

		+0.5f, +0.5f, +0.5f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, // Top
		-0.5f, +0.5f, +0.5f, 1.0f, 0.0f, 0.0f, +1.0f, 0.0f,
		-0.5f, +0.5f, -0.5f, 1.0f, 1.0f, 0.0f, +1.0f, 0.0f,
		+0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, +1.0f, 0.0f,

		+0.5f, -0.5f, +0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Bottom
		-0.5f, -0.5f, +0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		+0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	unsigned int tVAO;

	glGenVertexArrays(1, &tVAO);
	glBindVertexArray(tVAO);

	unsigned int VBO3;
	glGenBuffers(1, &VBO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(viis), viis, GL_DYNAMIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return tVAO;
}