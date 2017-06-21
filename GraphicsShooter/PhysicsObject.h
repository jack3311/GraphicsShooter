//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    PhysicsObject.h
//  Description  :    Represents a game object with physics details
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once
#include "Object.h"

class PhysicsObject :
	public Object
{
private:
	glm::vec3 velocity;
	float lifetime = 0.f;

public:
	PhysicsObject();
	PhysicsObject(glm::vec3 & _position, glm::vec3 & _scale, glm::mat4 & _rotation = glm::mat4());
	virtual ~PhysicsObject();

	glm::vec3 & getVelocity();
	void setVelocity(glm::vec3);

	///
	/// update:
	/// Updates the PhysicsObject
	/// _dt - the delta time
	///
	void update(float _dt);

	///
	/// getLifetime:
	/// Returns the duration that this object has existed for
	///
	float getLifetime() const;


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

	std::string bulletSource;	//Good enough solution, would normally create child class
								//Intentionally do not serialise/deserialise
};

