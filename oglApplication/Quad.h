#pragma once
#include "stdafx.h"
#include "3rd\glew\include\GL\glew.h"
#include "3rd\freeImage\FreeImage.h"
#include "3rd\glm\glm.hpp"
#include "3rd\glm\gtc\matrix_transform.hpp"
#include <vector>
using namespace std;
class EffectParameter;
class Quad
{
	GLuint
		VaoId,
		VertexVAOID[1],
		VertexVBOID[3],
		IndexVBOID[2],
		ColorBufferId;
	GLfloat *Vertices;
	GLfloat *Colors;
	GLfloat *texCoords;
	glm::vec4 quadRect;
public:
	Quad(void);
	void Draw(void);
	void Draw(int, int, int, int, EffectParameter *world);
	~Quad(void);
};

