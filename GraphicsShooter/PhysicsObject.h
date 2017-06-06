#pragma once
#include "Object.h"

class PhysicsObject :
	public Object
{
private:
	glm::vec3 velocity;
	float lifetime = 0.f;

public:
	PhysicsObject(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	virtual ~PhysicsObject();

	glm::vec3 & getVelocity();
	void setVelocity(glm::vec3);

	void update(float _dt);
};

