#include <sstream>

#include "MeshRenderer.h"

#include "Light.h"
#include "Camera.h"

#include "AssetManager.h"


MeshRenderer::MeshRenderer(GLuint _shader, std::vector<TexturedNormalsVertexFormat> _vertices, std::vector<GLuint> _indices, std::vector<ModelTextureInfo> _textures) : m_Shader(_shader)
{
	vertices = _vertices;
	indices = _indices;
	textures = _textures;
}

MeshRenderer::MeshRenderer(GLuint _shader, std::vector<TexturedNormalsVertexFormat> _vertices, std::vector<GLuint> _indices, GLuint _texture) : m_Shader(_shader)
{
	vertices = _vertices;
	indices = _indices;
	texture = _texture;
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialise()
{
	//SETUP VAO
	glGenVertexArrays(1, &m_QuadVAO);
	glBindVertexArray(m_QuadVAO);

	//SETUP VBO
	glGenBuffers(1, &m_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedNormalsVertexFormat), &vertices.front(), GL_STATIC_DRAW);

	//SETUP EBO
	glGenBuffers(1, &m_QuadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	//SETUP VERTEX ATTRIBUTES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalsVertexFormat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalsVertexFormat), (GLvoid*)offsetof(TexturedNormalsVertexFormat, TexturedNormalsVertexFormat::normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalsVertexFormat), (GLvoid*)offsetof(TexturedNormalsVertexFormat, TexturedNormalsVertexFormat::texCoord));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

//When not using ModelRenderer
void MeshRenderer::DrawMesh(const Camera & _camera, const glm::mat4 & _model, 
	const std::vector<Light*>& _lights, const int & _numLights, const float & _ambientStrength, const glm::vec3 _ambientColor, const float & _specularStrength) const
{
	glUseProgram(m_Shader);

	GLuint uniformProjectionLocation = glGetUniformLocation(m_Shader, "projection");
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(_camera.getProjMatrix()));

	GLuint uniformViewLocation = glGetUniformLocation(m_Shader, "view");
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(_camera.getViewMatrix()));

	GLuint uniformModelLocation = glGetUniformLocation(m_Shader, "model");
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(_model));


	//Lighting

	glm::vec3 * lightPositions = new glm::vec3[_numLights];
	glm::vec3 * lightColors = new glm::vec3[_numLights];
	for (int l = 0; l < _numLights; ++l)
	{
		lightPositions[l] = _lights[l]->getPosition();
		lightColors[l] = _lights[l]->getColor();
	}

	GLuint uniformLightPos1Location = glGetUniformLocation(m_Shader, "lightPositions");
	glUniform3fv(uniformLightPos1Location, _numLights, glm::value_ptr(lightPositions[0]));

	GLuint uniformLightCol1Location = glGetUniformLocation(m_Shader, "lightColors");
	glUniform3fv(uniformLightCol1Location, _numLights, glm::value_ptr(lightColors[0]));

	delete[] lightPositions;
	delete[] lightColors;

	GLuint uniformNumLightsLocation = glGetUniformLocation(m_Shader, "numLights");
	glUniform1i(uniformNumLightsLocation, _numLights);


	GLuint uniformCamPos = glGetUniformLocation(m_Shader, "camPos");
	glUniform3fv(uniformCamPos, 1, glm::value_ptr(_camera.getPosition()));

	GLuint uniformAmbientStrengthLocation = glGetUniformLocation(m_Shader, "ambientStrength");
	glUniform1f(uniformAmbientStrengthLocation, _ambientStrength);

	GLuint uniformAmbientColLocation = glGetUniformLocation(m_Shader, "ambientCol");
	glUniform3fv(uniformAmbientColLocation, 1, glm::value_ptr(_ambientColor));

	GLuint uniformSpecularStrength = glGetUniformLocation(m_Shader, "specularStrength");
	glUniform1f(uniformSpecularStrength, _specularStrength);

	//Draw

	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));

	glBindVertexArray(0);
}

//When using ModelRenderer
void MeshRenderer::DrawMeshSimple() const
{
	// Bind appropriate textures
	GLuint shaderTextureTypeCounter[ModelTextureType::MODELTEXTURETYPE_NUM_ITEMS];
	ZeroMemory(&shaderTextureTypeCounter, sizeof(shaderTextureTypeCounter));

	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding

		std::stringstream ss;

		ss << "texture_" << textures[i].type << "_" << shaderTextureTypeCounter[textures[i].type];

		++shaderTextureTypeCounter[textures[i].type];

		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(m_Shader, ss.str().c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}


	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0));
}