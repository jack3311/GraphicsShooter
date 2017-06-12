//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Camera.h
//  Description  :    Represents the camera
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"

#define VERTICAL_SCALE 1280
#define Z_NEAR 0.001f
#define Z_FAR 100000.f

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 lookAt;

	glm::mat4 view, proj;

public:
	Camera(glm::vec2 & _centerPosition);
	~Camera();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjMatrix() const;

	/// 
	/// transform:
	/// Transforms a matrix by the Proj and View matrices associated with the camera
	/// _source - the original matrix
	/// returns the new matrix
	///
	glm::mat4 transform(glm::mat4 & _source) const;

	/// 
	/// unproject:
	/// Unprojects a 2D screen coordinate to 2D world coordinates
	/// _projected - the screen coordinates
	/// returns the world coordinates
	///
	glm::vec2 unproject(glm::vec2 & _projected) const;

	/// 
	/// updateCamera:
	/// Recalculates the view and projection matrices for the camera
	///
	void updateCamera();

	void setPosition(glm::vec3 _position);
	void setLookAt(glm::vec3 _lookAtPos);

	const glm::vec3 & getPosition() const;
};

