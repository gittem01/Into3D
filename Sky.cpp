#include "Sky.h"

#define n 4000

Sky::Sky() {
	this->VAO = this->getDefaultVAO();
	this->starShader = new Shader("starShaders/");
	this->rotationAxis = glm::vec3(1, 1, 0);
	this->currentSkyAngle = 0.0f;
}

void Sky::update(Camera3D* cam) {
	this->starShader->use();
	glm::mat4 pers = cam->pers;
	glm::mat4 view = cam->getView(0);
	glm::mat4 rotationMatrix = this->getRotationMatrix();

	this->starShader->setMat4("pers", pers);
	this->starShader->setMat4("view", view);
	this->starShader->setMat4("rotationMatrix", rotationMatrix);
	this->starShader->setFloat("screenRatio", (float)cam->w/cam->h);
	this->starShader->setFloat("zoom", cam->zoom);
	glDepthMask(false);
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_POINTS, 0, n);
	glDepthMask(true);
	currentSkyAngle += 0.000f;
}

glm::mat4 Sky::getRotationMatrix() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, currentSkyAngle, this->rotationAxis);
	return model;
}

unsigned int Sky::getDefaultVAO() {
	float viis[n * 4];
	for (int i = 0; i < n; i++) {
		float v1 = ((float)rand() / RAND_MAX - 0.5);
		float v2 = ((float)rand() / RAND_MAX - 0.5);
		float v3 = ((float)rand() / RAND_MAX - 0.5);
		glm::vec3 p = glm::normalize(glm::vec3(v1, v2, v3));
		viis[i * 4] = p.x;
		viis[i * 4 + 1] = p.y;
		viis[i * 4 + 2] = p.z;
		viis[i * 4 + 3] = (float)rand() / RAND_MAX + 0.05;
	}

	unsigned int tVAO;
	
	glGenVertexArrays(1, &tVAO);
	glBindVertexArray(tVAO);

	unsigned int VBO3;
	glGenBuffers(1, &VBO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(viis), viis, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return tVAO;
}