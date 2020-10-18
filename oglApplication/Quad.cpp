#include "stdafx.h"
#include "Quad.h"
#include "3rd\glm\gtc\matrix_transform.hpp"
#include "3rd\glm\glm.hpp"
#include "Effect.h"

Quad::Quad(void)
{
	Vertices = new GLfloat[16];
	Vertices[0] = -1.0f;
	Vertices[1] = -1.0f;
	Vertices[2] = 0.0f;
	Vertices[3] = 1.0f;

	Vertices[4] = -1.0f;
	Vertices[5] = 1.0f;
	Vertices[6] = 0.0f;
	Vertices[7] = 1.0f;

	Vertices[8] = 1.0f;
	Vertices[9] = -1.0f;
	Vertices[10] = 0.0f;
	Vertices[11] = 1.0f;

	Vertices[12] = 1.0f;
	Vertices[13] = 1.0f;
	Vertices[14] = 0.0f;
	Vertices[15] = 1.0f;

	texCoords = new GLfloat[8];

	texCoords[0] = 0.0f;
	texCoords[1] = 1.0f;

	texCoords[2] = 0.0f;
	texCoords[3] = 0.0f;

	texCoords[4] = 1.0f;
	texCoords[5] = 1.0f;

	texCoords[6] = 1.0f;
	texCoords[7] = 0.0f;

	glGenVertexArrays(1, &VertexVAOID[0]);

	glBindVertexArray(VertexVAOID[0]);
	glGenBuffers(3, VertexVBOID);

	glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID[0]);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID[2]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);
}

void Quad::Draw()
{
	glBindVertexArray(VertexVAOID[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Quad::Draw(int pos_x, int pos_y, int width, int height, EffectParameter *world)
{
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	float tscalex = (float)width / (float)m_viewport[2];
	float tscaley = (float)height / (float)m_viewport[3];
	float tposx = (tscalex - 1);
	float tposy = -(tscaley - 1);
	tposx += ((float)2 * pos_x / (float)m_viewport[2]);
	tposy -= ((float)2 * pos_y / (float)m_viewport[3]);
	world->SetValue(glm::translate(glm::mat4(1.0f), glm::vec3(tposx, -tposy, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(tscalex, tscaley, 1.0f)));
	glBindVertexArray(VertexVAOID[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Quad::~Quad(void)
{
}
