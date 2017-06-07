#pragma once

#include <string>
#include <vector>
#include <functional>

#include "TextLabel.h"
#include "SpriteRenderer.h"

struct UIElement
{
	int x, y;
	int width, height;
	float scaleFactor = 1.f;
	std::string text;

	std::function<void()> * onClick = nullptr;

	UIElement(int _x, int _y, int _width, int _height, std::string _text, std::function<void()> * _onClick = nullptr);

	void draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const;
	void update(int _mX, int _mY);
	bool containsPoint(int _mX, int _mY) const;
};

class UIMenu
{
private:
	std::vector<UIElement *> uiElements;

	bool mouseStatusLastFrame = false;

	TextLabel * genericLabel = nullptr;
	SpriteRenderer * spriteRenderer = nullptr;
	GLuint blackTexture;


public:
	UIMenu();
	~UIMenu();

	void initialise();

	void addElement(UIElement * _element);
	void update(float _dt);
	void draw() const;
};

