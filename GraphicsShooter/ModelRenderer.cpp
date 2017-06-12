#include <iostream>

#include "ModelRenderer.h"
#include "AssetManager.h"
#include "Camera.h"
#include "Light.h"
#include "ScenePlay.h"


ModelRenderer::ModelRenderer(GLuint _shader, float _reflectionStrength) : m_Shader(_shader), reflectionStrength(_reflectionStrength)
{
}

ModelRenderer::~ModelRenderer()
{
}

bool ModelRenderer::loadFromFile(std::string _path)
{
	loadedTextures.clear();

	rootPath = _path.substr(0, _path.find_last_of('/'));

	Assimp::Importer imp;
	const aiScene * scene = imp.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << imp.GetErrorString() << std::endl;
		return false;
	}

	processNode(scene->mRootNode, scene);

	loadedTextures.clear();

	return true;
}

void ModelRenderer::processNode(aiNode * _node, const aiScene * _scene)
{
	//Process meshes
	for (unsigned int i = 0; i < _node->mNumMeshes; ++i)
	{
		aiMesh * mesh = _scene->mMeshes[_node->mMeshes[i]];
		processMesh(mesh, _scene);
	}

	//Process child nodes
	for (unsigned int i = 0; i < _node->mNumChildren; ++i)
	{
		processNode(_node->mChildren[i], _scene);
	}
}

void ModelRenderer::processMesh(aiMesh * _mesh, const aiScene * _scene)
{
	std::vector<TexturedNormalsVertexFormat> vertices(_mesh->mNumVertices);
	std::vector<GLuint> indices;
	std::vector<ModelTextureInfo> textures;

	//For each vertex of mesh
	for (unsigned int i = 0; i < _mesh->mNumVertices; ++i)
	{
		//If no tex coords, use 0, 0
		TexCoord texCoords = (_mesh->mTextureCoords[0]) ? TexCoord{ _mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y } : TexCoord{ 0, 0 };

		vertices[i] = TexturedNormalsVertexFormat{
			Position{ _mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z },
			Normal{ _mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z },
			texCoords
		};
	}

	//For each face (index set)
	for (unsigned int i = 0; i < _mesh->mNumFaces; ++i)
	{
		auto face = _mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//For material
	if (_mesh->mMaterialIndex >= 0)
	{
		//If there is a texture
		auto material = _scene->mMaterials[_mesh->mMaterialIndex];

		loadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, ModelTextureType::DIFFUSE, textures);
		loadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, ModelTextureType::SPECULAR, textures);
	}

	//Append meshRenderer
	meshRenderers.push_back(new MeshRenderer(m_Shader, vertices, indices, textures));
}

void ModelRenderer::loadMaterialTextures(aiMaterial * _material, aiTextureType _aiTextureType, ModelTextureType _textureType, std::vector<ModelTextureInfo> & _appendTextures)
{
	unsigned int numTextures = _material->GetTextureCount(_aiTextureType);
	for (unsigned int i = 0; i < numTextures; ++i)
	{
		aiString fileName;
		_material->GetTexture(_aiTextureType, i, &fileName);

		auto searchItr = std::find_if(loadedTextures.begin(), loadedTextures.end(), [&fileName](const ModelTextureInfo & _a) {
			return _a.fileName == fileName;
		});

		if (searchItr == loadedTextures.end())
		{
			//Need to load texture
			GLuint texID = AssetManager::getAssetManager().loadTextureSimple(rootPath + "/" + fileName.C_Str());
			ModelTextureInfo t{ texID, _textureType, fileName };
			_appendTextures.push_back(t);
			loadedTextures.push_back(t);
		}
		else
		{
			//Texture already loaded
			_appendTextures.push_back(*searchItr);
		}
	}
}

void ModelRenderer::initialize()
{
	for (const auto & mr : meshRenderers)
	{
		mr->Initialise();
	}
}

void ModelRenderer::draw(const Camera & _camera, const glm::mat4 & _model, const ScenePlay & _scenePlay) const
{
	glUseProgram(m_Shader);

	//MVP matrices

	GLuint uniformProjectionLocation = glGetUniformLocation(m_Shader, "projection");
	glUniformMatrix4fv(uniformProjectionLocation, 1, GL_FALSE, glm::value_ptr(_camera.getProjMatrix()));

	GLuint uniformViewLocation = glGetUniformLocation(m_Shader, "view");
	glUniformMatrix4fv(uniformViewLocation, 1, GL_FALSE, glm::value_ptr(_camera.getViewMatrix()));

	GLuint uniformModelLocation = glGetUniformLocation(m_Shader, "model");
	glUniformMatrix4fv(uniformModelLocation, 1, GL_FALSE, glm::value_ptr(_model));


	//Lighting:

	unsigned int numLights = _scenePlay.getNumLights();
	auto lights = _scenePlay.getLights();

	glm::vec3 * lightPositions = new glm::vec3[numLights];
	glm::vec3 * lightColors = new glm::vec3[numLights];
	glm::vec3 * lightSpotDirs = new glm::vec3[numLights];
	float * lightSpotAngles = new float[numLights];
	for (unsigned int l = 0; l < numLights; ++l)
	{
		lightPositions[l] = lights[l]->getPosition();
		lightColors[l] = lights[l]->getColor();
		lightSpotDirs[l] = lights[l]->getConeDir();
		lightSpotAngles[l] = lights[l]->getConeAngle();
	}

	GLuint uniformLightPos1Location = glGetUniformLocation(m_Shader, "lightPositions");
	glUniform3fv(uniformLightPos1Location, numLights, glm::value_ptr(lightPositions[0]));

	GLuint uniformLightCol1Location = glGetUniformLocation(m_Shader, "lightColors");
	glUniform3fv(uniformLightCol1Location, numLights, glm::value_ptr(lightColors[0]));

	GLuint uniformLightSpotDirsLocation = glGetUniformLocation(m_Shader, "lightSpotDirs");
	glUniform3fv(uniformLightSpotDirsLocation, numLights, glm::value_ptr(lightSpotDirs[0]));

	GLuint uniformLightSpotAnglesLocation = glGetUniformLocation(m_Shader, "lightSpotAngles");
	glUniform1fv(uniformLightSpotAnglesLocation, numLights, &lightSpotAngles[0]);


	delete[] lightPositions;
	delete[] lightColors;
	delete[] lightSpotDirs;
	delete[] lightSpotAngles;


	GLuint uniformNumLightsLocation = glGetUniformLocation(m_Shader, "numLights");
	glUniform1i(uniformNumLightsLocation, numLights);

	GLuint uniformCamPos = glGetUniformLocation(m_Shader, "camPos");
	glUniform3fv(uniformCamPos, 1, glm::value_ptr(_camera.getPosition()));

	GLuint uniformAmbientStrengthLocation = glGetUniformLocation(m_Shader, "ambientStrength");
	glUniform1f(uniformAmbientStrengthLocation, _scenePlay.getAmbientStrength());

	GLuint uniformAmbientColLocation = glGetUniformLocation(m_Shader, "ambientCol");
	glUniform3fv(uniformAmbientColLocation, 1, glm::value_ptr(_scenePlay.getAmbientColor()));

	GLuint uniformSpecularStrength = glGetUniformLocation(m_Shader, "specularStrength");
	glUniform1f(uniformSpecularStrength, _scenePlay.getSpecularStrength());


	//Reflections:

	GLuint uniformReflectionStrengthLocation = glGetUniformLocation(m_Shader, "reflectionStrength");
	glUniform1f(uniformReflectionStrengthLocation, reflectionStrength);


	//Assign skybox tex
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_Shader, "skybox"), 0);
	glBindTexture(GL_TEXTURE_2D, _scenePlay.getSkyboxCubemap());


	//Draw all of the meshes

	for (const auto & mr : meshRenderers)
	{
		mr->DrawMeshSimple();
	}
}
