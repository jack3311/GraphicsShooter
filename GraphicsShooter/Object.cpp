//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Object.cpp
//  Description  :    Represents a game object
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Object.h"

#include "Light.h"

Object::Object()
{
}

Object::Object(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation) :
	position(_position), scale(_scale), rotation(_rotation), playerTarget("")
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

const glm::vec3 & Object::getScale() const
{
	return scale;
}

const glm::vec3 & Object::getPosition() const
{
	return position;
}

const glm::mat4 & Object::getRotation() const
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

void Object::serialise(std::ostringstream & _oss)
{
	//Flag
	_oss << flag << " ";

	//Position
	vec3ToStream(_oss, position);

	//Scale
	vec3ToStream(_oss, scale);

	//Rotation
	mat4ToStream(_oss, rotation);
}

void Object::deserialise(std::istringstream & _iss)
{
	_iss >> flag;
	position = vec3FromStream(_iss);
	scale = vec3FromStream(_iss);
	rotation = mat4FromStream(_iss);
}
