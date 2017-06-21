//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    UIMenu.h
//  Description  :    Represents a menu
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "TextLabel.h"
#include "SpriteRenderer.h"

#define ELEMENT_PADDING_X 24
#define ELEMENT_PADDING_Y 18

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

	///
	/// draw:
	/// Draws this UI element
	/// _texture - the texture of the element background
	/// _spriteRenderer - the sprite renderer
	/// _label - a generic text label
	///
	void draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const;

	///
	/// update:
	/// Updates this UI element
	/// _mX, _mY - the mouse X and Y coordinates
	///
	void update(int _mX, int _mY);

	///
	/// containsPoint:
	/// Returns whether this UI element contains a given X and Y coordinates
	/// _mX, _mY - the X and Y coordinates
	///
	bool containsPoint(int _mX, int _mY) const;

	///
	/// recalculateWH:
	/// Recalculates the width and height of this element
	/// _label - a generic text label
	///
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

	///
	/// initialise:
	/// Sets up this UIMenu
	///
	void initialise();

	///
	/// addElement:
	/// Adds an element to the menu
	/// _element - a pointer to the element
	///
	void addElement(UIElement * _element);

	///
	/// update:
	/// Updates the menu
	/// _dt - the delta time
	///
	void update(float _dt);

	///
	/// draw:
	/// Draws the menu
	///
	void draw() const;

	///
	/// clear:
	/// Removes all existing elements from the menu
	///
	void clear();
};

