#include "Object.h"

#include "Light.h"

Object::Object(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation) : 
	position(_position), scale(_scale), rotation(_rotation)
{
}

Object::~Object()
{
}

void Object::setRotation(glm::mat4 _rotation)
{
	rotation = _rotation;
}

void Object::setScale(glm::vec3 _scale)
{
	scale = _scale;
}

void Object::setPosition(glm::vec3 _position)
{
	position = _position;
}

void Object::move(glm::vec3 delta)
{
	position += delta;
}

glm::vec3 & Object::getScale()
{
	return scale;
}

glm::vec3 & Object::getPosition()
{
	return position;
}

glm::mat4 & Object::getRotation()
{
	return rotation;
}

void Object::lookAt(glm::vec3 pos)
{
	if (pos == position) return; //Ignore if we are at the same position

	auto forwards = glm::vec3(0, 0, 1);
	auto delta = glm::normalize(pos - position);
	
	float dot = glm::dot(forwards, delta);
	float angle = acosf(dot);
	auto axis = glm::cross(forwards, delta);

	rotation = glm::rotate(glm::mat4(), angle, axis);
}

const glm::mat4 Object::getModelMatrix() const
{
	const glm::mat4 step1 = glm::scale(glm::mat4(), scale);
	const glm::mat4 & step2 = rotation;
	const glm::mat4 step3 = glm::translate(glm::mat4(), position);

	return step3 * step2 * step1;
}
