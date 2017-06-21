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
#include <string>
#include <cassert>
#include <sstream>

///
/// vec3FromStream:
/// Gets a vec3 from a stringstream
/// _iss - the stringstream
/// returns the vec3
///
glm::vec3 vec3FromStream(std::istringstream & _iss);

///
/// vec3ToStream:
/// Writes a vec3 to a stringstream
/// _oss - the stringstream
/// _val - the vec3
///
void vec3ToStream(std::ostringstream & oss, const glm::vec3 & _val);

///
/// mat4FromStream:
/// Gets a mat4 from a stringstream
/// _iss - the stringstream
/// returns the mat4
///
glm::mat4 mat4FromStream(std::istringstream & _iss);

///
/// mat4ToStream:
/// Writes a mat4 to a stringstream
/// _oss - the stringstream
/// _val - the mat4
///
void mat4ToStream(std::ostringstream & _oss, const glm::mat4 & _val);

///
/// dist2:
/// Calculates the square of the distance between two points
/// x1, y1 - the first point's coordinates
/// x2, y2 - the second point's coordinates
/// returns the square distance between the points
///
template <typename T>
T dist2(T x1, T y1, T x2, T y2)
{
	T dX = x1 - x2;
	T dY = y1 - y2;

	return dX * dX + dY * dY;
}

///
/// concatenate
/// Joins a set of strings together
/// _first - the first string
/// _args - the remaining strings
///
template <typename T, typename... Args>
std::string concatenate(T _first, Args ..._args)
{
	return std::string(_first) + concatenate(_args...);
}

///
/// concatenate
/// Joins a set of strings together
/// _first - the first string
///
template <typename T>
std::string concatenate(T _first)
{
	return std::string(_first);
}

///
/// clamp
/// Ensures a value is in a given range
/// _min - the infimum of the range
/// _val - the value to clamp
/// _max - the supremum of the range
/// returns the clamped value
///
template <typename T>
T clamp(T _min, T _val, T _max)
{
	T temp = _val;
	temp = temp > _min ? temp : _min;
	temp = temp < _max ? temp : _max;
	return temp;
}

///
/// length2:
/// Calculates the square of the length of a vector
/// x - the vector
/// returns the square of the vector's length
///
template <typename T, glm::precision P, template <typename, glm::precision> class vecType>
GLM_FUNC_QUALIFIER T length2(vecType<T, P> const & x)
{
	GLM_STATIC_ASSERT(std::numeric_limits<T>::is_iec559, "'dot' only accept floating-point inputs");
	return glm::detail::compute_dot<vecType, T, P>::call(x, x);
}

///
/// circleCircleCollision:
/// Calculates the collision between two circles
/// _pos1 - the position of the first circle
/// _r1 - the radius of the first circle
/// _pos2 - the position of the second circle
/// _r2 - the radius of the second circle
///
inline bool circleCircleCollision(const glm::vec2 & _pos1, const float & _r1, const glm::vec2 & _pos2, const float & _r2)
{
	float r1plusr2 = _r1 + _r2;
	auto d = _pos1 - _pos2;
	return length2(d) < r1plusr2 * r1plusr2;
}

///
/// lineCircleCollision:
/// Calculates the collision between a line segment and a circle
/// _lineStartPos - the start position of the line
/// _lineEndPos - the end position of the line
/// _lineRadius - the radius of the line
/// _circlePos - the position of the circle
/// _r - the radius of the circle
///
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

///
/// sphereSphereCollision:
/// Calculates the collision between two spheres
/// _pos1 - the position of the first sphere
/// _r1 - the radius of the first sphere
/// _pos2 - the position of the second sphere
/// _r2 - the radius of the second sphere
///
inline bool sphereSphereCollision(const glm::vec3 & _pos1, const float & _r1, const glm::vec3 & _pos2, const float & _r2)
{
	float r1plusr2 = _r1 + _r2;
	auto d = _pos1 - _pos2;
	return length2(d) < r1plusr2 * r1plusr2;
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