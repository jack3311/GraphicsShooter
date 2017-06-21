//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Object.h
//  Description  :    Represents a game object
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#include <string>

#include "Camera.h"
#include "ModelRenderer.h"

class Object
{
private:
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 rotation;

public:
	Object();
	Object(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	~Object();

	void setRotation(glm::mat4 _rotation);
	void setScale(glm::vec3 _scale);
	void setPosition(glm::vec3 _position);

	/// 
	/// move:
	/// Moves the position of the model by a given vector
	/// delta - the movement vector
	///
	void move(glm::vec3 delta);

	const glm::vec3 & getScale() const;
	const glm::vec3 & getPosition() const;
	const glm::mat4 & getRotation() const;

	/// 
	/// lookAt:
	/// Sets the rotation of the model to look at a given point
	/// pos - the point to look at
	///
	void lookAt(glm::vec3 pos);

	const glm::mat4 getModelMatrix() const;

	unsigned int flag;

	std::string playerTarget;

	/// 
	/// serialise:
	/// Converts information about the object to a packet
	/// _oss - an ostringstream for the packet
	///
	virtual void serialise(std::ostringstream & _oss);

	/// 
	/// deserialise:
	/// Sets information about the object from a packet
	/// _iss - an istringstream for the packet
	///
	virtual void deserialise(std::istringstream & _iss);
};

