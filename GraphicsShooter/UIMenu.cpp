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
	blackTexture = AssetManager::getAssetManager().loadTexture("buttonBg", "Assets/Textures/bullet.jpg");


	ShaderLoader sl;
	GLuint shader = sl.CreateProgram("Shaders/texturedSpriteShader.vert",
		"Shaders/texturedSpriteShader.frag");/*
	GLuint shader = sl.CreateProgram("Shaders/uiBgShader.vert",
		"Shaders/uiBgShader.frag");*/

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
		e->draw(blackTexture, spriteRenderer, genericLabel);
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

UIElement::UIElement(float _x, float _y, int _width, int _height, std::string _text, std::function<void()> _onClick) :
	x(_x), y(_y), width(_width), height(_height), text(_text), onClick(_onClick)
{
}

UIElement::UIElement(float _x, float _y, std::string _text, std::function<void()> _onClick) : x(_x), y(_y), text(_text), onClick(_onClick)
{
	calcWH = true; //Calculate width and height based on text label
}

void UIElement::draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const
{
	float actualX = x * Game::getGame()->getWindowWidth();
	float actualY = y * Game::getGame()->getWindowHeight();

	_label->setPosition(glm::vec2(actualX, actualY));
	_label->setText(text);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Game::getGame()->getWindowWidth()),
		0.0f, static_cast<GLfloat>(Game::getGame()->getWindowHeight()));

	auto scale = glm::scale(glm::mat4(), scaleFactor * glm::vec3(width, height, 1));
	auto translate = glm::translate(glm::mat4(), glm::vec3(actualX, actualY, 0.f));

	_spriteRenderer->DrawSprite(_texture, projection, glm::mat4(), translate * scale);

	_label->Render();
}

void UIElement::update(int _mX, int _mY)
{
	if (containsPoint(_mX, _mY))
	{
		scaleFactor = 1.5f;
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
