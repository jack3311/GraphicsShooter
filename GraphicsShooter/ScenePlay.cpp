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

#include <sstream>

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
	ambientStrength = 0.6f;
	ambientColor = glm::vec3(1.f, 0.8f, 0.8f);
	specularStrength = 0.8f;

	ShaderLoader sl;
	GLuint shader = sl.CreateProgram("Shaders/texturedNormalsShader.vert",
		"Shaders/texturedNormalsShader.frag");


	/*std::vector<TexturedNormalsVertexFormat> tempVertices;
	std::vector<GLuint> tempIndices;
	Util::CreateTexturedNormalsCube(tempVertices, tempIndices);
	meshRendererTest = new MeshRenderer(shader, tempVertices, tempIndices, AssetManager::getAssetManager().getTexture("planet1"));
	meshRendererTest->Initialise();*/


	std::vector<TexturedNormalsVertexFormat> tempFloorVertices;
	std::vector<GLuint> tempFloorIndices;
	Util::CreateTexturedNormalsCube(tempFloorVertices, tempFloorIndices);
	floorRenderer = new MeshRenderer(shader, tempFloorVertices, tempFloorIndices,
		AssetManager::getAssetManager().loadTexture("bullet", "Assets/Textures/bullet.jpg"));
	floorRenderer->Initialise();


	playerRenderer = new ModelRenderer(shader, 0.0f);
	playerRenderer->loadFromFile("Assets/Models/tank1/Abrams_BF3.obj");
	//modelRendererTest->loadFromFile("Assets/Models/gun/Handgun_obj.obj");
	playerRenderer->initialize();

	enemy1Renderer = new ModelRenderer(shader, 0.0f);
	enemy1Renderer->loadFromFile("Assets/Models/tank2/tank2.obj");
	//modelRendererTest->loadFromFile("Assets/Models/gun/Handgun_obj.obj");
	enemy1Renderer->initialize();
	

	std::vector<TexturedNormalsVertexFormat> tempBulletVertices;
	std::vector<GLuint> tempBulletIndices;
	Util::CreateSphere(tempBulletVertices, tempBulletIndices);
	//Util::CreateTexturedNormalsCube(tempBulletVertices, tempBulletIndices);
	bulletRenderer = new MeshRenderer(shader, tempBulletVertices, tempBulletIndices,
		AssetManager::getAssetManager().loadTexture("bullet", "Assets/Textures/bullet.jpg"));
	bulletRenderer->Initialise();

	std::vector<TexturedNormalsVertexFormat> tempPowerup1Vertices;
	std::vector<GLuint> tempPowerup1Indices;
	Util::CreateTexturedNormalsCube(tempPowerup1Vertices, tempPowerup1Indices);
	powerup1Renderer = new MeshRenderer(shader, tempPowerup1Vertices, tempPowerup1Indices,
		AssetManager::getAssetManager().loadTexture("bullet", "Assets/Textures/bullet.jpg"));
	powerup1Renderer->Initialise();



	GLuint shader1 = sl.CreateProgram("Shaders/skyboxShader.vert",
		"Shaders/skyboxShader.frag");

	skyboxRenderer = new SkyboxRenderer(shader1);
	skyboxRenderer->Initialise();


	lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.5f, 1.f)));
	lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(1.f, 0.5f, 0.f)));

	ammoText = new TextLabel("", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), false);
	scoreText = new TextLabel("", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), false);
	healthText = new TextLabel("", "Assets/Fonts/arial.ttf", glm::vec2(0, 0), false);
}

ScenePlay::~ScenePlay()
{
	//TODO: Add destructor...
}

void ScenePlay::render() const
{
	auto & gw = Game::getGame()->getGameWorld();

	//Render floor
	auto floorScale = glm::scale(glm::mat4(), glm::vec3(10000.f, 1.f, 10000.f));
	auto floorTrans = glm::translate(glm::mat4(), glm::vec3(0.f, -2.f, 0.f));

	floorRenderer->DrawMesh(Game::getGame()->getCamera(), floorTrans * floorScale, *this, 0.1f);

	//Render skybox
	skyboxRenderer->DrawModel(AssetManager::getAssetManager().getCubeMap("skybox1"), Game::getGame()->getCamera(), glm::scale(glm::mat4(), glm::vec3(10000.f, 10000.f, 10000.f)));

	//Render player
	playerRenderer->draw(Game::getGame()->getCamera(), gw.getPlayer()->getModelMatrix(), *this);

	//Render enemies
	for (auto enemy : gw.getEnemies())
	{
		enemy1Renderer->draw(Game::getGame()->getCamera(), enemy->getModelMatrix(), *this);
	}

	//Render bullets
	for (auto bullet : gw.getBullets())
	{
		bulletRenderer->DrawMesh(Game::getGame()->getCamera(), bullet->getModelMatrix(), *this, 0.0f);
	}

	//Render powerups
	for (auto powerup : gw.getPowerups())
	{
		powerup1Renderer->DrawMesh(Game::getGame()->getCamera(), powerup->getModelMatrix(), *this, 0.0f);
	}

	//Render text
	scoreText->Render();
	ammoText->Render();
	healthText->Render();
}

void ScenePlay::update(float _dt)
{
	auto & gw = Game::getGame()->getGameWorld();
	auto thisPlayer = gw.getPlayer();

	elapsedTime += _dt;




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
		thisPlayer->setRotation(glm::rotate(thisPlayer->getRotation(), 3.f * _dt, glm::vec3(0, 1, 0)));
	}

	if (Input::isKeyDown('d'))
	{
		thisPlayer->setRotation(glm::rotate(thisPlayer->getRotation(), -3.f * _dt, glm::vec3(0, 1, 0)));
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

	if (Input::isKeyDown(' ') && !spaceBarLastFrame)
	{
		gw.playerFire();
	}

	spaceBarLastFrame = Input::isKeyDown(' ');

	if (Input::isKeyDown(27)) //Escape key
	{
		SceneManager::getSceneManager().activate<SceneMenu>();
	}


	//Update game world
	Game::getGame()->getGameWorld().update(_dt);



	//Camera settings
	Game & g = *Game::getGame();

	auto offset = glm::cross(facing2D, glm::vec3(0, 1, 0)) * 2.f + glm::vec3(0.f, 3.5f, 0);
	offset *= thisPlayer->getScale();
	g.getCamera().setPosition(thisPlayer->getPosition() + offset - facing2D * 6.f);
	g.getCamera().setLookAt(thisPlayer->getPosition() + facing2D * 100.f);


	Game::getGame()->getCamera().updateCamera();




	//Light testing

	//lights.at(0)->setPosition(g.getCamera().getPosition());
	lights.at(1)->setPosition(glm::vec3(cosf(elapsedTime) * 4.f, 0.f, -sinf(elapsedTime) * 4.f));

	//ambientStrength = 0.5f * (sinf(elapsedTime / 2.f) + 1.f);


	//Update text
	std::stringstream ss;
	if (thisPlayer->isReloading())
	{
		ss << "Reloading...";
	}
	else if (thisPlayer->hasInfiniteAmmo())
	{
		ss << "Infinite Ammo";
	}
	else
	{
		ss << "Ammo: " << thisPlayer->getAmmo();
	}
	ammoText->setText(ss.str());


	ss = std::stringstream();
	ss << "Score: " << thisPlayer->getScore();
	scoreText->setText(ss.str());

	ss = std::stringstream();
	ss << "Health: " << thisPlayer->getHealth() << " / " << MAX_HEALTH;
	if (thisPlayer->hasShield())
	{
		ss << " SHIELDED";
	}
	healthText->setText(ss.str());

	ammoText->setPosition(glm::vec2(0.f, 0.f));
	healthText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() - healthText->getWidth(), 0.f));

	scoreText->setPosition(glm::vec2(Game::getGame()->getWindowWidth() / 2.f - scoreText->getWidth() / 2.f,
		Game::getGame()->getWindowHeight() - scoreText->getHeight()));
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
