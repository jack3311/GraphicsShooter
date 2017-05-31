//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Light.cpp
//  Description  :    Represents a light
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Light.h"

Light::Light(glm::vec3 _position, glm::vec3 _colour) : position(_position), colour(_colour)
{
}

Light::~Light()
{
}

const glm::vec3 & Light::getPosition() const
{
	return position;
}

void Light::setPosition(glm::vec3 _position)
{
	position = _position;
}

const glm::vec3 & Light::getColor() const
{
	return colour;
}