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

public:
	ModelRenderer(GLuint _shader, float _reflectionStrength = 0.2f);
	~ModelRenderer();

	bool loadFromFile(std::string _path);
	void processNode(aiNode * _node, const aiScene * _scene);
	void processMesh(aiMesh * _mesh, const aiScene * _scene);
	void loadMaterialTextures(aiMaterial * _material, aiTextureType _aiTextureType, ModelTextureType _textureType, std::vector<ModelTextureInfo> & _appendTextures);

	bool initialize();

	void draw(const Camera & _camera, const glm::mat4 & _model, const ScenePlay & _scenePlay) const;
};