#include "SkyboxRenderer.h"

#include "Light.h"
#include "Camera.h"

SkyboxRenderer::SkyboxRenderer(GLuint _shader) : m_Shader(_shader)
{
	Util::CreateCube(vertices, indices);
}

SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::Initialise()
{
	//SETUP VAO
	glGenVertexArrays(1, &m_QuadVAO);
	glBindVertexArray(m_QuadVAO);

	//SETUP VBO
	glGenBuffers(1, &m_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Position), &vertices.front(), GL_STATIC_DRAW);

	//SETUP EBO
	glGenBuffers(1, &m_QuadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	//SETUP VERTEX ATTRIBUTES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void SkyboxRenderer::DrawModel(GLuint _texture, const Camera & _camera, const glm::mat4 & _model) const
{
	//glEnable(GL_TEXTURE_CUBE_MAP);
	//glDepthMask(GL_FALSE);

	glUseProgram(m_Shader);

	GLuint uniformProjectionLocation = glGetUniformLocation(m_Shader, "projection");
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(_camera.getProjMatrix()));

	GLuint uniformViewLocation = glGetUniformLocation(m_Shader, "view");
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(_camera.getViewMatrix()));

	GLuint uniformModelLocation = glGetUniformLocation(m_Shader, "model");
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(_model));



	//Draw

	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);


	/*glActiveTexture(GL_TEXTURE0);
	GLuint uniformSkyboxLocation = glGetUniformLocation(m_Shader, "skybox");
	glUniform1i(uniformSkyboxLocation, 0);*/
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);



	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));

	glBindVertexArray(0);

	//glDepthMask(GL_TRUE);
	//glDisable(GL_TEXTURE_CUBE_MAP);
}