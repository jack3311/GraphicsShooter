//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    MeshRenderer.h
//  Description  :    Renders mesh objects
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
#include "ModelInfo.h"


class Light;
class Camera;
class ScenePlay;

class MeshRenderer
{
protected:
	GLuint m_QuadVAO;
	GLuint m_QuadVBO;
	GLuint m_QuadEBO;

	std::vector<TexturedNormalsVertexFormat> vertices;
	std::vector<GLuint> indices;

	GLuint m_Shader;

	union
	{
		std::vector<ModelTextureInfo> textures; //When using ModelRenderer

		GLuint texture;
	};

public:
	/// 
	/// MeshRenderer:
	/// Creates a mesh renderer associated with a model
	/// _shader - the mesh shader
	/// _vertices - the set of vertices
	/// _indices - the set of indices
	/// _textures - the set of model textures
	///
	MeshRenderer(GLuint _shader, std::vector<TexturedNormalsVertexFormat> _vertices, std::vector<GLuint> _indices, std::vector<ModelTextureInfo> _textures);

	/// 
	/// MeshRenderer:
	/// Creates a standalone mesh renderer
	/// _shader - the mesh shader
	/// _vertices - the set of vertices
	/// _indices - the set of indices
	/// _texture - the mesh texture
	///
	MeshRenderer(GLuint _shader, std::vector<TexturedNormalsVertexFormat> _vertices, std::vector<GLuint> _indices, GLuint _texture);
	~MeshRenderer();

	/// 
	/// Initialise:
	/// Initilises the mesh renderer
	///
	void Initialise();

	/// 
	/// DrawMesh:
	/// Draws the standalone mesh
	/// _camera - the game camera
	/// _model - the model matrix
	/// _scenePlay - the play scene
	/// _reflectionStrength - the strength of reflections on this mesh
	///
	void DrawMesh(const Camera & _camera, const glm::mat4 & _model, const ScenePlay & _scenePlay, const float & _reflectionStrength) const;

	/// 
	/// DrawMesh:
	/// Draws the model-associated mesh
	///
	void DrawMeshSimple() const;
};

