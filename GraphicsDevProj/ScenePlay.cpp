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

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtx\projection.hpp"

#include <sstream>


ScenePlay::ScenePlay()
{
	ambientStrength = 0.4f;
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


	modelRendererTest = new ModelRenderer(shader, lights, numLights, ambientStrength, ambientColor, specularStrength);
	//modelRendererTest->loadFromFile("Assets/Models/castle/Castle OBJ.obj");
	modelRendererTest->loadFromFile("Assets/Models/gun/Handgun_obj.obj");
	modelRendererTest->initialize();


	GLuint shader1 = sl.CreateProgram("Shaders/skyboxShader.vert",
		"Shaders/skyboxShader.frag");

	skyboxRenderer = new SkyboxRenderer(shader1);
	skyboxRenderer->Initialise();


	lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.5f, 1.f)));
	//lights.push_back(new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(1.f, 0.5f, 0.f)));

	numLights = lights.size();
}

ScenePlay::~ScenePlay()
{
}

void ScenePlay::render() const
{
	skyboxRenderer->DrawModel(AssetManager::getAssetManager().getCubeMap("skybox1"), Game::getGame()->getCamera(), glm::scale(glm::mat4(), glm::vec3(100.f, 100.f, 100.f)));


	Object o = Object(glm::vec3(0, 0, 0), glm::vec3(1.f, 1.f, 1.f));
	//meshRendererTest->DrawMesh(Game::getGame()->getCamera(), o.getModelMatrix(), lights, numLights, ambientStrength, ambientColor, specularStrength);

	modelRendererTest->draw(Game::getGame()->getCamera(), o.getModelMatrix());
}

void ScenePlay::update(float _dt)
{
	elapsedTime += _dt;

	if (Input::isKeyDown('a'))
	{
	}

	if (Input::isKeyDown('d'))
	{
	}

	if (Input::isKeyDown('s'))
	{
	}

	if (Input::isKeyDown('w'))
	{
	}


	Game & g = *Game::getGame();

	//g.getCamera().setPosition(glm::vec3(-3.0f, 7.0f, 5.f));
	g.getCamera().setPosition(glm::vec3(sinf(elapsedTime / 2.f) * 4.f, sinf(elapsedTime / 2.f) * 3.f, cosf(elapsedTime / 2.f) * 4.f));
	g.getCamera().setLookAt(glm::vec3(0.f, 0.f, 0.f));

	Game::getGame()->getCamera().updateCamera();


	lights.at(0)->setPosition(g.getCamera().getPosition());
	//lights.at(1)->setPosition(glm::vec3(cosf(elapsedTime) * 4.f, 0.f, -sinf(elapsedTime) * 4.f));

	//ambientStrength = 0.5f * (sinf(elapsedTime / 2.f) + 1.f);
}

void ScenePlay::reset()
{
}