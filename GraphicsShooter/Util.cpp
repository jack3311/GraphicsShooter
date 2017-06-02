//
//  Bachelor of Game Development
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c) 2017 Media Design School
//
//  File Name    :    Util.cpp
//  Description  :    Utility functions and classes
//  Author       :    Jack Wilson
//  Mail         :    jack.wil6883@mediadesign.school.nz
//

#include "Util.h"

void floatToChar(float _in, char _out[sizeof(float)])
{
	union {
		float a;
		char b[sizeof(float)];
	} conversionUnion;

	conversionUnion.a = _in;
	memcpy_s(_out, sizeof(_out), conversionUnion.b, sizeof(conversionUnion.b));
}

float charToFloat(char _in[sizeof(float)])
{
	union {
		float a;
		char b[sizeof(float)];
	} conversionUnion;

	memcpy_s(conversionUnion.b, sizeof(conversionUnion.b), _in, sizeof(_in));

	return conversionUnion.a;
}

void Util::CreateQuad(std::vector<TexturedVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices, float _fWidth, float _fHeight)
{
	_rVertices = {
		TexturedVertexFormat{ Position{ -_fWidth, _fHeight, 0.0f },	Color(), TexCoord{ 0.0f, 0.0f } }, //Top Left
		TexturedVertexFormat{ Position{ _fWidth, _fHeight, 0.0f }, Color(), TexCoord{ 1.0f, 0.0f } }, //Top Right
		TexturedVertexFormat{ Position{ _fWidth, -_fHeight, 0.0f },	Color(), TexCoord{ 1.0f, 1.0f } }, //Bottom Right
		TexturedVertexFormat{ Position{ -_fWidth, -_fHeight, 0.0f }, Color(), TexCoord{ 0.0f, 1.0f } }, //Bottom Left
	};
	_rIndices = {
		0, 1, 2,
		0, 2, 3
	};
}

void Util::CreateQuad(std::vector<VertexFormat>& _rVertices, std::vector<GLuint>& _rIndices, float _fWidth, float _fHeight)
{
	_rVertices = {
		VertexFormat{ Position{ -_fWidth, _fHeight, 0.0f }, Color() },
		VertexFormat{ Position{ _fWidth, _fHeight, 0.0f }, Color() },
		VertexFormat{ Position{ _fWidth, -_fHeight, 0.0f }, Color() },
		VertexFormat{ Position{ -_fWidth, -_fHeight, 0.0f }, Color() }
	};
	_rIndices = {
		0, 1, 2,
		0, 2, 3
	};
}

void Util::CreateTriangle(std::vector<TexturedVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices)
{
	_rVertices = {
		TexturedVertexFormat{ Position{ -0.5f, 0.5f, 0.0f }, Color{ 1.0f, 1.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedVertexFormat{ Position{ 0.5f, 0.5f, 0.0f }, Color{ 1.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedVertexFormat{ Position{ 0.5f, -0.5f, 0.0f }, Color{ 0.0f, 1.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
	};
	_rIndices = {
		0, 1, 2
	};
}

void Util::CreatePyramid(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices)
{
	_rVertices = {
		TexturedNormalsVertexFormat{ Position{ -1.0f, 0.0f, 1.0f }, Normal{ 0.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0f, 0.0f, -1.0f }, Normal{ 0.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0f, 0.0f, -1.0f }, Normal{ 0.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0f, 0.0f, 1.0f }, Normal{ 0.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },

		TexturedNormalsVertexFormat{ Position{ 0.0f, 1.0f, 0.0f }, Normal{ 0.0f, 0.0f, 0.0f }, TexCoord{ 0.5f, 0.5f } },
	};
	_rIndices = {
		0, 1, 2,
		0, 2, 3,
		0, 4, 3,
		1, 4, 0,
		2, 4, 1,
		3, 4, 2
	};
}

void Util::CreateSphere(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices)
{
	static const int rings = 30;
	static const int sectors = 30;
	static const float radius = 1.f;

	float const R = 1.f / (float)(rings - 1);
	float const S = 1.f / (float)(sectors - 1);
	int r, s;

	_rVertices.resize(rings * sectors * 3);
	auto v = _rVertices.begin();

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-glm::pi<float>() / 2.f + glm::pi<float>() * r * R);
			float const x = cos(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
			float const z = sin(2 * glm::pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

			Position p{ x * radius, y * radius, z * radius };
			Normal n{ x, y, z };
			TexCoord t{ s*S, r*R };

			*v++ = TexturedNormalsVertexFormat{ p, n, t };
		}
	}

	_rIndices.resize(rings * sectors * 6);
	auto i = _rIndices.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);

			*i++ = r * sectors + s;
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}
}

void Util::CreateTexturedNormalsCube(std::vector<TexturedNormalsVertexFormat>& _rVertices, std::vector<GLuint>& _rIndices)
{
	_rVertices = {
		// front
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, 1.0}, Normal{ 0.0f, 0.0f, 1.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, 1.0 }, Normal{ 0.0f, 0.0f, 1.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, 1.0 }, Normal{ 0.0f, 0.0f, 1.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, 1.0 }, Normal{ 0.0f, 0.0f, 1.0f }, TexCoord{ 0.0f, 1.0f } },
		// right
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, 1.0 }, Normal{ 1.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, -1.0 }, Normal{ 1.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, -1.0 }, Normal{ 1.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, 1.0 }, Normal{ 1.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 1.0f } },
		// back
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, -1.0 }, Normal{ 0.0f, 0.0f, -1.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, -1.0 }, Normal{ 0.0f, 0.0f, -1.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, -1.0 }, Normal{ 0.0f, 0.0f, -1.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, -1.0 }, Normal{ 0.0f, 0.0f, -1.0f }, TexCoord{ 0.0f, 1.0f } },
		// left
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, -1.0 }, Normal{ -1.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, 1.0 }, Normal{ -1.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, 1.0 }, Normal{ -1.0f, 0.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, -1.0 }, Normal{ -1.0f, 0.0f, 0.0f }, TexCoord{ 0.0f, 1.0f } },
		// top
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, 1.0 }, Normal{ 0.0f, 1.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, 1.0 }, Normal{ 0.0f, 1.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, 1.0, -1.0 }, Normal{ 0.0f, 1.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, 1.0, -1.0 }, Normal{ 0.0f, 1.0f, 0.0f }, TexCoord{ 0.0f, 1.0f } },
		// bottom
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, -1.0 }, Normal{ 0.0f, -1.0f, 0.0f }, TexCoord{ 0.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, -1.0 }, Normal{ 0.0f, -1.0f, 0.0f }, TexCoord{ 1.0f, 0.0f } },
		TexturedNormalsVertexFormat{ Position{ 1.0, -1.0, 1.0 }, Normal{ 0.0f, -1.0f, 0.0f }, TexCoord{ 1.0f, 1.0f } },
		TexturedNormalsVertexFormat{ Position{ -1.0, -1.0, 1.0 }, Normal{ 0.0f, -1.0f, 0.0f }, TexCoord{ 0.0f, 1.0f } }
	};

	_rIndices = {
		0, 1, 2, 0, 2, 3,		//front
		4, 5, 6, 4, 6, 7,		//right
		8, 9, 10, 8, 10, 11,	//back
		12, 13, 14, 12, 14, 15, //left
		16, 17, 18, 16, 18, 19, //upper
		20, 21, 22, 20, 22, 23	//bottom
	};
}

void Util::CreateCube(std::vector<Position>& _rVertices, std::vector<GLuint>& _rIndices)
{
	_rVertices = {
		// front
		Position{ -1.0, -1.0, 1.0 },
		Position{ 1.0, -1.0, 1.0 },
		Position{ 1.0, 1.0, 1.0 },
		Position{ -1.0, 1.0, 1.0 },
		// right
		Position{ 1.0, 1.0, 1.0 },
		Position{ 1.0, 1.0, -1.0 },
		Position{ 1.0, -1.0, -1.0 },
		Position{ 1.0, -1.0, 1.0 },
		// back
		Position{ -1.0, -1.0, -1.0 },
		Position{ 1.0, -1.0, -1.0 },
		Position{ 1.0, 1.0, -1.0 },
		Position{ -1.0, 1.0, -1.0 },
		// left
		Position{ -1.0, -1.0, -1.0 },
		Position{ -1.0, -1.0, 1.0 },
		Position{ -1.0, 1.0, 1.0 },
		Position{ -1.0, 1.0, -1.0 },
		// top
		Position{ 1.0, 1.0, 1.0 },
		Position{ -1.0, 1.0, 1.0 },
		Position{ -1.0, 1.0, -1.0 },
		Position{ 1.0, 1.0, -1.0 },
		// bottom
		Position{ -1.0, -1.0, -1.0 },
		Position{ 1.0, -1.0, -1.0 },
		Position{ 1.0, -1.0, 1.0 },
		Position{ -1.0, -1.0, 1.0 }
	};

	_rIndices = {
		0, 1, 2, 0, 2, 3,		//front
		4, 5, 6, 4, 6, 7,		//right
		8, 9, 10, 8, 10, 11,	//back
		12, 13, 14, 12, 14, 15, //left
		16, 17, 18, 16, 18, 19, //upper
		20, 21, 22, 20, 22, 23	//bottom
	};
}

void Util::CreateTriangle(std::vector<VertexFormat>& _rVertices, std::vector<GLuint>& _rIndices)
{
	_rVertices = {
		VertexFormat{ Position{ -0.75f, 0.5f, 0.0f }, Color{ 1.0f, 1.0f, 0.0f } },
		VertexFormat{ Position{ 0.5f, 0.5f, 0.0f }, Color{ 1.0f, 0.0f, 0.0f } },
		VertexFormat{ Position{ 0.5f, -0.5f, 0.0f }, Color{ 0.0f, 1.0f, 0.0f } },
	};
	_rIndices = {
		0, 1, 2
	};
}