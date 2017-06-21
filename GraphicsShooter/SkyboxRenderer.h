//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SkyboxRenderer.h
//  Description  :    Renders skyboxes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <vector>

#include "Util.h"

class Light;
class Camera;

class SkyboxRenderer
{
protected:
	GLuint m_QuadVAO;
	GLuint m_QuadVBO;
	GLuint m_QuadEBO;

	GLuint m_Shader;

	std::vector<Position> vertices;
	std::vector<GLuint> indices;


public:
	SkyboxRenderer(GLuint _shader);
	~SkyboxRenderer();

	/// 
	/// Initialise:
	/// Initilises the skybox renderer
	///
	void Initialise();

	/// 
	/// DrawModel:
	/// Draws the skybox
	/// _texture - the skybox texture
	/// _camera - the game camera
	/// _model - the model matrix
	///
	void DrawModel(GLuint _texture, const Camera & _camera, const glm::mat4 & _model) const;
};

