//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    ScenePlay.cpp
//  Description  :    The play scene
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "ScenePlay.h"

#include "AssetManager.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "Input.h"
#include "Game.h"
#include "Util.h"
#include "Camera.h" 
#include "Object.h"
#include "Light.h"
#include "GameWorld.h"

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtx\projection.hpp"


ScenePlay::ScenePlay()
{
	ambientStrength = 1.0f;
	ambientColor = glm::vec3(1.f, 0.8f, 0.8f);
	specularStrength = 1.f;

	ShaderLoader sl;
	GLuint shader = sl.CreateProgram("Shaders/texturedNormalsShader.vert",
		"Shaders/texturedNormalsShader.frag");


	/*std::vector<TexturedNormalsVertexFormat> tempVertices;
	std::vector<GLuint> tempIndices;
	Util::CreateTexturedNormalsCube(tempVertices, tempIndices);
	meshRendererTest = new MeshRenderer(shader, tempVertices, tempIndices, AssetManager::getAssetManager().getTexture("planet1"));
	meshRendererTest->Initialise();*/


	modelRendererTest = new ModelRenderer(shader, 0.0f);
	modelRendererTest->loadFromFile("Assets/Models/tank1/Abrams_BF3.obj");
	//modelRendererTest->loadFromFile("Assets/Models/gun/Handgun_obj.obj");
	modelRendererTest->initialize();


	GLuint shader1 = sl.CreateProgram("Shaders/skyboxShader.vert",
		"Shaders/skyboxShader.frag");

	skyboxRenderer = new SkyboxRenderer(shader1);
	skyboxRenderer->Initialise();


	lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.5f, 1.f)));
	lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(1.f, 0.5f, 0.f)));
	players.push_back(new Object(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::mat4()));
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::render() const
{
	skyboxRenderer->DrawModel(AssetManager::getAssetManager().getCubeMap("skybox1"), Game::getGame()->getCamera(), glm::scale(glm::mat4(), glm::vec3(100.f, 100.f, 100.f)));

	for (auto player : players)
	{
		modelRendererTest->draw(Game::getGame()->getCamera(), player->getModelMatrix(), *this);
	}

	auto & gw = Game::getGame()->getGameWorld();



	modelRendererTest->draw(Game::getGame()->getCamera(), gw.testObject->getModelMatrix(), *this);
}

void ScenePlay::update(float _dt)
{
	elapsedTime += _dt;
	auto thisPlayer = players[0];


	//Mouse controls
	/*glm::vec2 mousePos = Input::getMousePosition();

	glm::vec2 mouseDel = mousePos - glm::vec2(Game::getGame()->getWindowWidth() / 2.f, Game::getGame()->getWindowHeight() / 2.f);

	//glutWarpPointer(Game::getGame()->getWindowWidth() / 2.f, Game::getGame()->getWindowHeight() / 2.f);

	float rotationX = -mouseDel.x * 0.005f;
	float rotationY = mouseDel.y * 0.005f;

	thisPlayer->setRotation(glm::rotate(thisPlayer->getRotation(), rotationX, glm::vec3(0, 1, 0)));*/



	//Keyboard controls
	if (Input::isKeyDown('a'))
	{
		thisPlayer->setRotation(glm::rotate(thisPlayer->getRotation(), 5.f * _dt, glm::vec3(0, 1, 0)));
	}

	if (Input::isKeyDown('d'))
	{
		thisPlayer->setRotation(glm::rotate(thisPlayer->getRotation(), -5.f * _dt, glm::vec3(0, 1, 0)));
	}

	auto facing2D = -glm::vec3(thisPlayer->getRotation() * glm::vec4(0, 0, 1.f, 0.f));

	if (Input::isKeyDown('s'))
	{
		thisPlayer->setPosition(thisPlayer->getPosition() - facing2D * 10.f * _dt);
	}

	if (Input::isKeyDown('w'))
	{
		thisPlayer->setPosition(thisPlayer->getPosition() + facing2D * 10.f * _dt);
	}


	//Update game world
	Game::getGame()->getGameWorld().update(_dt);






	//Camera settings
	Game & g = *Game::getGame();

	auto offset = glm::cross(facing2D, glm::vec3(0, 1, 0)) * 2.f + glm::vec3(0.f, 3.5f, 0);
	g.getCamera().setPosition(thisPlayer->getPosition() + offset - facing2D * 8.f);
	g.getCamera().setLookAt(thisPlayer->getPosition() + facing2D * 800.f);



	/*g.getCamera().setLookAt(glm::vec3(0, 0, 0));
	g.getCamera().setPosition(glm::vec3(-30.0f, 70.0f, 50.f));*/


	Game::getGame()->getCamera().updateCamera();




	//Light testing

	//lights.at(0)->setPosition(g.getCamera().getPosition());
	lights.at(1)->setPosition(glm::vec3(cosf(elapsedTime) * 4.f, 0.f, -sinf(elapsedTime) * 4.f));

	//ambientStrength = 0.5f * (sinf(elapsedTime / 2.f) + 1.f);
}

void ScenePlay::reset()
{
}




const std::vector<Light*>& ScenePlay::getLights() const
{
	return lights;
}

const unsigned int ScenePlay::getNumLights() const
{
	return lights.size();
}

const float ScenePlay::getAmbientStrength() const
{
	return ambientStrength;
}

const glm::vec3 & ScenePlay::getAmbientColor() const
{
	return ambientColor;
}

const float ScenePlay::getSpecularStrength() const
{
	return specularStrength;
}

const GLuint ScenePlay::getSkyboxCubemap() const
{
	return AssetManager::getAssetManager().getCubeMap("skybox1"); //TODO: maybe change this
}
