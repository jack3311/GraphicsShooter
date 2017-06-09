#pragma once

#include <string>
#include <vector>
#include <functional>

#include "TextLabel.h"
#include "SpriteRenderer.h"

#define ELEMENT_PADDING_X 24.f
#define ELEMENT_PADDING_Y 18.f

struct UIElement
{
	float x, y;
	int width, height;
	float scaleFactor = 1.f;
	std::string text;
	bool calcWH = false;
	bool isButton;

	std::function<void()> onClick;

	UIElement(float _x, float _y, int _width, int _height, std::string _text, bool _isButton = true, std::function<void()> _onClick = []() {});
	UIElement(float _x, float _y, std::string _text, bool _isButton = true, std::function<void()> _onClick = []() {});

	void draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const;
	void update(int _mX, int _mY);
	bool containsPoint(int _mX, int _mY) const;
	void recalculateWH(TextLabel * _label);
};

class UIMenu
{
private:
	std::vector<UIElement *> uiElements;

	bool mouseStatusLastFrame = false;

	TextLabel * genericLabel = nullptr;
	SpriteRenderer * spriteRenderer = nullptr;
	GLuint buttonTexture;


public:
	UIMenu();
	~UIMenu();

	void initialise();

	void addElement(UIElement * _element);
	void update(float _dt);
	void draw() const;
	void clear();
};

