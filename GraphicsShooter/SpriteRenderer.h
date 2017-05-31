//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SpriteRenderer.h
//  Description  :    Renders sprite objects
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

class SpriteRenderer
{
protected:
	GLuint m_QuadVAO;
	GLuint m_QuadVBO;
	GLuint m_QuadEBO;

	GLuint m_Shader;

	std::vector<TexturedVertexFormat> vertices;
	std::vector<GLuint> indices;

public:
	SpriteRenderer(GLuint _shader);
	~SpriteRenderer();

	void Initialise();

	void DrawSprite(GLuint _texture, glm::mat4 & _projection, glm::mat4 & _view, glm::mat4 & _model) const;
};

