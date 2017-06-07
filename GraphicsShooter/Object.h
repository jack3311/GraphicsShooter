//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Sprite.h
//  Description  :    Represents a game object
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#include "Camera.h"
#include "ModelRenderer.h"

class Object
{
private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 rotation;

public:
	Object(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	~Object();

	void setRotation(glm::mat4 _rotation);
	void setScale(glm::vec3 _scale);
	void setPosition(glm::vec3 _position);

	void move(glm::vec3 delta);

	glm::vec3 & getScale();
	glm::vec3 & getPosition();
	glm::mat4 & getRotation();

	void lookAt(glm::vec3 pos);

	const glm::mat4 getModelMatrix() const;
};

