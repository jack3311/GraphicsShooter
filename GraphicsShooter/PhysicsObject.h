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


	virtual void serialise(std::ostringstream & _oss);
	virtual void deserialise(std::istringstream & _iss);
};

