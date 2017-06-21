//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    ModelRenderer.h
//  Description  :    Renders models
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <vector>

#include "ModelInfo.h"
#include "MeshRenderer.h"

class ScenePlay;

class ModelRenderer
{
private:
	std::vector<MeshRenderer *> meshRenderers;

	std::vector<ModelTextureInfo> loadedTextures;

	std::string rootPath;

	GLuint m_Shader;

	float reflectionStrength;

	/// 
	/// processNode:
	/// Processes a node of a model
	///
	void processNode(aiNode * _node, const aiScene * _scene);

	/// 
	/// processMesh:
	/// Processes a mesh of a model
	///
	void processMesh(aiMesh * _mesh, const aiScene * _scene);

	/// 
	/// loadMaterialTextures:
	/// Loads all material textures
	///
	void loadMaterialTextures(aiMaterial * _material, aiTextureType _aiTextureType, ModelTextureType _textureType, std::vector<ModelTextureInfo> & _appendTextures);

public:
	ModelRenderer(GLuint _shader, float _reflectionStrength = 0.2f);
	~ModelRenderer();

	/// 
	/// loadFromFile:
	/// Loads the model from a file
	/// _path - the path of the model
	/// returns whether the load was successful
	///
	bool loadFromFile(std::string _path);

	/// 
	/// initialize:
	/// Initilises the model renderer
	///
	void initialize();

	/// 
	/// draw:
	/// Draws the model
	/// _camera - the game camera
	/// _model - the model matrix
	/// _scenePlay - the play scene
	///
	void draw(const Camera & _camera, const glm::mat4 & _model, const ScenePlay & _scenePlay) const;
};