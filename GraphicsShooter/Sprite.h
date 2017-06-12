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
#include "SpriteRenderer.h"

class Sprite
{
private:
	GLuint texture;
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;

protected:
	bool shouldDelete = false;

public:
	Sprite(GLuint _texture, glm::vec2 & _position, glm::vec2 & _scale, const float & _rotation);
	~Sprite();
	
	///
	/// draw:
	/// Draws the sprite to a sprite renderer
	/// _spriteRenderer - the sprite renderer
	/// _camera - the game camera
	///
	virtual void draw(SpriteRenderer & _spriteRenderer, Camera & _camera);

	void setRotation(float _radians);
	void setScale(glm::vec2 _scale);
	void setPosition(glm::vec2 _position);

	void move(glm::vec2 delta);

	glm::vec2 & getScale();
	glm::vec2 & getPosition();
	float & getRotation();

	bool getShouldDelete() const;
	void setShouldDelete(bool _value);

	int flag;
};

