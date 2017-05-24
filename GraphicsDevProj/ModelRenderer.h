#pragma once

#include <vector>

#include "ModelInfo.h"
#include "MeshRenderer.h"

class ModelRenderer
{
private:
	std::vector<MeshRenderer *> meshRenderers;

	std::vector<ModelTextureInfo> loadedTextures;

	std::string rootPath;

	GLuint m_Shader;

	//TODO: FIX THIS NONSENSE
	const std::vector<Light *> & lights;
	const int & numLights;
	const float & ambientStrength;
	const glm::vec3 ambientColor;
	const float & specularStrength;

public:
	ModelRenderer(GLuint _shader, const std::vector<Light*>& _lights, const int & _numLights, const float & _ambientStrength, const glm::vec3 _ambientColor, const float & _specularStrength);
	~ModelRenderer();

	bool loadFromFile(std::string _path);
	void processNode(aiNode * _node, const aiScene * _scene);
	void processMesh(aiMesh * _mesh, const aiScene * _scene);
	void loadMaterialTextures(aiMaterial * _material, aiTextureType _aiTextureType, ModelTextureType _textureType, std::vector<ModelTextureInfo> & _appendTextures);

	bool initialize();

	void draw(const Camera & _camera, const glm::mat4 & _model) const;
};