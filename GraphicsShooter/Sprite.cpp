//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Sprite.cpp
//  Description  :    Represents a game object
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Sprite.h"

Sprite::Sprite(GLuint _texture, glm::vec2 & _position, glm::vec2 & _scale, const float & _rotation) : texture(_texture), position(_position), scale(_scale), rotation(_rotation)
{
}

Sprite::~Sprite()
{
}

void Sprite::draw(SpriteRenderer & _spriteRenderer, Camera & _camera)
{
	glm::mat4 step1 = glm::scale(glm::mat4(), glm::vec3(scale, 1.f));
	glm::mat4 step2 = glm::rotate(glm::mat4(), rotation, glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 step3 = glm::translate(glm::mat4(), glm::vec3(-position.x, position.y, 0.f));

	_spriteRenderer.DrawSprite(texture, _camera.getProjMatrix(), _camera.getViewMatrix(),  step3 * step2 * step1);
}

void Sprite::setRotation(float _radians)
{
	rotation = _radians;
}

void Sprite::setScale(glm::vec2 _scale)
{
	scale = _scale;
}

void Sprite::setPosition(glm::vec2 _position)
{
	position = _position;
}

void Sprite::move(glm::vec2 delta)
{
	position += delta;
}

glm::vec2 & Sprite::getScale()
{
	return scale;
}

glm::vec2 & Sprite::getPosition()
{
	return position;
}

float & Sprite::getRotation()
{
	return rotation;
}

bool Sprite::getShouldDelete() const
{
	return shouldDelete;
}

void Sprite::setShouldDelete(bool _value)
{
	shouldDelete = _value;
}
