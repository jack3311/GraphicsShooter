//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    SpriteRenderer.cpp
//  Description  :    Renders sprite objects
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(GLuint m_Shader) : m_Shader(m_Shader)
{

}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Initialise()
{
	Util::CreateQuad(vertices, indices);

	//SETUP VAO
	glGenVertexArrays(1, &m_QuadVAO);
	glBindVertexArray(m_QuadVAO);

	//SETUP VBO
	glGenBuffers(1, &m_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertexFormat), &vertices.front(), GL_STATIC_DRAW);
	
	//SETUP EBO
	glGenBuffers(1, &m_QuadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	//SETUP VERTEX ATTRIBUTES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void SpriteRenderer::DrawSprite(GLuint _texture, glm::mat4 & _projection, glm::mat4 & _view, glm::mat4 & _model) const
{
	glUseProgram(m_Shader);

	GLuint uniformProjectionLocation = glGetUniformLocation(m_Shader, "projection");
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(_projection));

	GLuint uniformViewLocation = glGetUniformLocation(m_Shader, "view");
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(_view));

	GLuint uniformModelLocation = glGetUniformLocation(m_Shader, "model");
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(_model));

	glBindTexture(GL_TEXTURE_2D, _texture);

	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));

	glBindVertexArray(0);
}
