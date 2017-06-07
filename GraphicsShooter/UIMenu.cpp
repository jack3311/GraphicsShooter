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
				if (e->onClick)
				{
					(*e->onClick)();
				}
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

UIElement::UIElement(int _x, int _y, int _width, int _height, std::string _text, std::function<void()> * _onClick) :
	x(_x), y(_y), width(_width), height(_height), text(_text), onClick(_onClick)
{
}

void UIElement::draw(GLuint _texture, SpriteRenderer * _spriteRenderer, TextLabel * _label) const
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Game::getGame()->getWindowWidth()),
		0.0f, static_cast<GLfloat>(Game::getGame()->getWindowHeight()));

	auto scale = glm::scale(glm::mat4(), scaleFactor * glm::vec3(width, height, 1));
	auto translate = glm::translate(glm::mat4(), glm::vec3(x, y, 0.f));

	_spriteRenderer->DrawSprite(_texture, projection, glm::mat4(), translate * scale);

	_label->setPosition(glm::vec2(x, y));
	_label->setText(text);
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
	return	_mX > x - (width / 2)	&& _mX < x + (width / 2) &&
			_mY > y - (height / 2)	&& _mY < y + (height / 2);
}
