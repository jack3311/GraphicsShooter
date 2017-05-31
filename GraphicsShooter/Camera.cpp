//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Camera.cpp
//  Description  :    Represents the camera
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Camera.h"

#include "Game.h"

Camera::Camera(glm::vec2 & _centerPosition) : position(glm::vec3(_centerPosition, -1.f)), lookAt(glm::vec3(_centerPosition, 0.f))
{
	updateCamera();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return view;
}

glm::mat4 Camera::getProjMatrix() const
{
	return proj;
}

glm::mat4 Camera::transform(glm::mat4 & _source) const
{
	return getProjMatrix() * getViewMatrix() * _source;
}

glm::vec2 Camera::unproject(glm::vec2 & _projected) const
{
	//Jack approach
	glm::vec2 scaledProjected = glm::vec2(
		_projected.x * 2.f / Game::getGame()->getWindowWidth() - 1.f,
		_projected.y * 2.f / Game::getGame()->getWindowHeight() - 1.f
	);

	glm::mat4 inverseViewProj = glm::inverse(getProjMatrix() * getViewMatrix());
	glm::vec4 projected4(scaledProjected, 0.f, 1.f);
	glm::vec4 world = inverseViewProj * projected4;
	world /= world.w;

	//Built-in approach using glm::unProject (uses model matrix which is ugly)
	/*
	glm::vec4 projected4(_projected, 0.f, 1.f);
	glm::vec3 windowCoords(_projected.x, _projected.y, 0.f);
	glm::vec4 viewport(0.f, 0.f, Game::getGame()->getWindowWidth(), Game::getGame()->getWindowHeight());
	glm::vec3 world = glm::unProject(windowCoords, glm::mat4(), getProjMatrix() * getViewMatrix(), viewport) * glm::vec3(-1.f, 1.f, 1.f);
	*/

	return glm::vec2(-world.x, world.y);
}

void Camera::updateCamera()
{
	float ratio = static_cast<float>(Game::getGame()->getWindowWidth()) / static_cast<float>(Game::getGame()->getWindowHeight());

	view = glm::lookAt(glm::vec3(position.x, position.y, position.z),
		glm::vec3(lookAt.x, lookAt.y, lookAt.z), glm::vec3(0.f, 1.f, 0.f));

	proj = glm::perspective(45.f, ratio, Z_NEAR, Z_FAR);
}

void Camera::setPosition(glm::vec3 _position)
{
	position = _position;
}

void Camera::setLookAt(glm::vec3 _lookAtPos)
{
	lookAt = _lookAtPos;
}

const glm::vec3 & Camera::getPosition() const
{
	return position;
}
