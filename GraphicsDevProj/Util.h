//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Util.h
//  Description  :    Utility functions and classes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtx\projection.hpp"

#include <vector>

template <typename T>
T clamp(T _min, T _val, T _max)
{
	T temp = _val;
	temp = temp > _min ? temp : _min;
	temp = temp < _max ? temp : _max;
	return temp;
}

template <typename T, glm::precision P, template <typename, glm::precision> class vecType>
GLM_FUNC_QUALIFIER T length2(vecType<T, P> const & x)
{
	GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' only accept floating-point inputs");
	return glm::detail::compute_dot<vecType, T, P>::call(x, x);
}

inline bool circleCircleCollision(const glm::vec2 & _pos1, const float & _r1, const glm::vec2 & _pos2, const float & _r2)
{
	float r1plusr2 = _r1 + _r2;
	auto d = _pos1 - _pos2;
	return length2(d) < r1plusr2 * r1plusr2;
}

inline bool lineCircleCollision(const glm::vec2 & _lineStartPos, const glm::vec2 & _lineEndPos, const float _lineRadius, const glm::vec2 & _circlePos, const float & _r)
{
	glm::vec2 diff = _circlePos - _lineStartPos;
	glm::vec2 dir = _lineEndPos - _lineStartPos;

	float t = glm::dot(diff, dir) / glm::dot(dir, dir);

	t = clamp(0.f, t, 1.f);

	glm::vec2 closest = _lineStartPos + t * dir;
	glm::vec2 dist = _circlePos - closest;
	float dist2 = length2(dist);
		
	return fabsf(dist2) <= powf((_lineRadius + _r), 2.f);
}

struct Position
{
	GLfloat x, y, z;

	Position() {}
	Position(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
};

struct TexCoord
{
	GLfloat u, v;

	TexCoord() {}
	TexCoord(GLfloat _u, GLfloat _v) : u(_u), v(_v) {}
};

struct Color
{
	GLfloat r, g, b;

	Color(GLfloat _r, GLfloat _g, GLfloat _b) : r(_r), g(_g), b(_b) {}
	Color() : r(1.f), g(1.f), b(1.f) {}
};

struct Normal
{
	GLfloat x, y, z;

	Normal() {}
	Normal(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
};

struct VertexFormat
{
	Position position;
	Color color;
	
	VertexFormat() {}
	VertexFormat(Position _position, Color _color) : position(_position), color(_color) {}
};

struct TexturedVertexFormat
{
	Position position;
	Color color;
	TexCoord texCoord;

	TexturedVertexFormat() {}
	TexturedVertexFormat(Position _position, Color _color, TexCoord _texCoord) : position(_position), color(_color), texCoord(_texCoord) {}
};

struct TexturedNormalsVertexFormat
{
	Position position;
	Normal normal;
	TexCoord texCoord;

	TexturedNormalsVertexFormat() {}
	TexturedNormalsVertexFormat(Position _position, Normal _normal, TexCoord _texCoord) : position(_position), normal(_normal), texCoord(_texCoord) {}
};

class Util
{
private:
	Util() = delete;
	Util(const Util&) = delete;
	~Util() = delete;

public:
	static void CreateQuad(std::vector<VertexFormat>& _rVertices, std::vector<GLuint>& _rIndices, float _fWidth = 0.5f, float _fHeight = 0.5f);
	static void CreateQuad(std::vector<TexturedVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices, float _fWidth = 0.5f, float _fHeight = 0.5f);


	static void CreateTriangle(std::vector<VertexFormat>& _rVertices, std::vector<GLuint>& _rIndices);
	static void CreateTriangle(std::vector<TexturedVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices);


	static void CreatePyramid(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices);


	static void CreateSphere(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices);

	static void CreateTexturedNormalsCube(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices);
	static void CreateCube(std::vector<Position>& _rVertices, std::vector<GLuint>& _rIndices);
};