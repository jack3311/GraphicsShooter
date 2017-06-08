#include "PhysicsObject.h"


PhysicsObject::PhysicsObject(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation) : Object(_position, _scale, _rotation)
{
}

PhysicsObject::~PhysicsObject()
{
}

glm::vec3 & PhysicsObject::getVelocity()
{
	return velocity;
}

void PhysicsObject::setVelocity(glm::vec3 _velocity)
{
	velocity = _velocity;
}

void PhysicsObject::update(float _dt)
{
	move(velocity * _dt);
	lifetime += _dt;
}

float PhysicsObject::getLifetime() const
{
	return lifetime;
}