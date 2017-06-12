//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Light.h
//  Description  :    Represents a light
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

class Light
{
private:
	glm::vec3 position;
	glm::vec3 colour;

	float coneAngle;
	glm::vec3 coneDir;

public:
	Light(glm::vec3 _position, glm::vec3 _colour);
	Light(glm::vec3 _position, glm::vec3 _colour, float _coneAngle, glm::vec3 _coneDir);
	~Light();

	const glm::vec3 & getPosition() const;
	void setPosition(glm::vec3 _position);

	const glm::vec3 & getColor() const;

	const float & getConeAngle() const;

	const glm::vec3 & getConeDir() const;
	void setConeDir(const glm::vec3 & _coneDir);

	bool isSpotlight() const;
};

