//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    ModelInfo.h
//  Description  :    Utilities to store models
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glew\wglew.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Util.h"

enum ModelTextureType
{
	DIFFUSE,
	SPECULAR,
	MODELTEXTURETYPE_NUM_ITEMS
};

struct ModelTextureInfo
{
	GLuint id;
	ModelTextureType type;
	aiString fileName;

	ModelTextureInfo(GLuint _id, ModelTextureType _type, aiString _fileName) :
		id(_id), type(_type), fileName(_fileName)
	{
	}
};