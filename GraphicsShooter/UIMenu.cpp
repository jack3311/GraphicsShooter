//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    UIMenu.cpp
//  Description  :    Represents a menu
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "UIMenu.h"

#include "Input.h"
#include "AssetManager.h"
#include "ShaderLoader.h"
#include "Game.h"



UIMenu::UIMenu()
{
}


UIMenu::~UIMenu()
{
	for (auto e : uiElements)
	{
		delete e;
	}

	delete spriteRenderer;
	delete genericLabel;
}

void UIMenu::initialise()
{
	buttonTexture = AssetManager::getAssetManager().getTexture("button");

	ShaderLoader sl;
	GLuint shader = sl.CreateProgram("Shaders/texturedSpriteShader.vert",
		"Shaders/texturedSpriteShader.frag");

	spriteRenderer = new SpriteRenderer(shader);
	spriteRenderer->Initialise();

	//Use just one label so we don't repeatedly load assets, create vbos, etc...
	genericLabel = new TextLabel("", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), true);
}

void UIMenu::addElement(UIElement * _element)
{
	uiElements.push_back(_element);
	if (_element->calcWH)
	{
		_element->recalculateWH(genericLabel);
	}
}

void UIMenu::update(float _dt)
{
	auto mousePos = Input::getMousePosition();
	int mX = static_cast<int>(mousePos.x);
	int mY = static_cast<int>(mousePos.y);

	for (auto e : uiElements)
	{
		e->update(mX, mY);
	}

	if (Input::isMouseDown(0) && !mouseStatusLastFrame)
	{
		for (auto e : uiElements)
		{
			if (e->containsPoint(mX, mY))
			{
				//Mouse down on this ui element
				e->onClick();

				//Play click sound
				AssetManager::getAssetManager().playSound(AssetManager::getAssetManager().getSound("click"));
				break; //Do not check other elements
			}
		}
	}

	mouseStatusLastFrame = Input::isMouseDown(0);
}

void UIMenu::draw() const
{
	for (auto e : uiElements)
	{
		e->draw(buttonTexture, spriteRenderer, genericLabel);
	}
}

void UIMenu::clear()
{
	for (auto e : uiElements)
	{
		delete e;
	}

	uiElements.clear();
}

UIElement::UIElement(float _x, float _y, int _width, int _height, std::string _text, bool _isButton, std::function<void()> _onClick) :
	x(_x), y(_y), width(_width), height(_height), text(_text), onClick(_onClick), isButton(_isButton)
{
}

UIElement::UIElement(float _x, float _y, std::string _text, bool _isButton, std::function<void()> _onClick) : 
	x(_x), y(_y), text(_text), onClick(_onClick), isButton(_isButton)
{
	calcWH = true; //Calculate width and height based on text label
}

void UIElement::draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const
{
	float actualX = x * Game::getGame()->getWindowWidth();
	float actualY = y * Game::getGame()->getWindowHeight();

	_label->setPosition(glm::vec2(actualX, actualY));
	_label->setText(text);
	_label->setScale(1.0f + (scaleFactor - 1.0f) / 2.f);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Game::getGame()->getWindowWidth()),
		0.0f, static_cast<GLfloat>(Game::getGame()->getWindowHeight()));

	auto scale = glm::scale(glm::mat4(), scaleFactor * glm::vec3(width, height, 1));
	auto translate = glm::translate(glm::mat4(), glm::vec3(actualX, actualY, 0.f));

	if (isButton)
		_spriteRenderer->DrawSprite(_texture, projection, glm::mat4(), translate * scale);

	_label->Render();
}

void UIElement::update(int _mX, int _mY)
{
	if (isButton && containsPoint(_mX, _mY))
	{
		float actualX = x * Game::getGame()->getWindowWidth();
		float actualY = y * Game::getGame()->getWindowHeight();

		float d2 = dist2<float>(static_cast<float>(_mX), static_cast<float>(_mY), actualX, actualY);
		float maxD2 = (width / 2.f) * (width / 2.f) + (height / 2.f) * (height / 2.f);

		float dP = d2 / maxD2;
		dP = 1.0f - dP;

		dP *= 0.25f;


		scaleFactor = 1.25f + dP;
	}
	else
	{
		scaleFactor = 1.f;
	}
}

bool UIElement::containsPoint(int _mX, int _mY) const
{
	float actualX = x * Game::getGame()->getWindowWidth();
	float actualY = y * Game::getGame()->getWindowHeight();

	return	_mX > actualX - (width / 2)		&& _mX < actualX + (width / 2) &&
			_mY > actualY - (height / 2)	&& _mY < actualY + (height / 2);
}

void UIElement::recalculateWH(TextLabel * _label)
{
	_label->setText(text);

	//Find width, height based on text
	width = _label->getWidth() + ELEMENT_PADDING_X;
	height = _label->getHeight() + ELEMENT_PADDING_Y;
}
