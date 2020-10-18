#pragma once
#include "stdafx.h"
#include "3rd\glew\include\GL\glew.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "3rd\GLFW\include\glfw3.h"
#include "3rd\glm\glm.hpp"
#include "3rd\glm\gtc\type_ptr.hpp"
#include <map>
#include "RenderTarget2D.h"

using namespace std;
class Texture2D;
class RenderTarget2D;


class EffectParameter
{
	GLuint paramID;
	GLenum type;
	GLuint textureID;

public:
	
	EffectParameter(){}
	
	EffectParameter(GLuint ProgramId, GLchar name[], GLenum u_type) : type(u_type)
	{
		if(this == NULL) { return; }
		paramID = glGetUniformLocation(ProgramId, name);
		textureID = -1;
	}
	
	EffectParameter(GLuint ProgramId, GLchar name[], GLenum u_type, GLuint u_textureID) : type(u_type), textureID(u_textureID)
	{
		if(this == NULL) { return; }
		paramID = glGetUniformLocation(ProgramId, name);
	}
	
	void SetValue(glm::mat4x4 value)
	{
		if(this == NULL) { return; }
		glUniformMatrix4fv(paramID, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	void SetValue(float value)
	{
		if(this == NULL) { return; }
		glUniform1f(paramID, value);
	}
	
	void SetValue(int value)
	{
		if(this == NULL) { return; }
		glUniform1i(paramID, value);
	}
	
	void SetValue(glm::vec2 value)
	{
		if(this == NULL) { return; }
		glUniform2fv(paramID, 1, glm::value_ptr(value));
	}
	
	void SetValue(glm::vec3 value)
	{
		if(this == NULL) { return; }
		glUniform3fv(paramID, 1, glm::value_ptr(value));
	}
	
	void SetValue(glm::vec4 value)
	{
		if(this == NULL) { return; }
		glUniform4fv(paramID, 1, glm::value_ptr(value));
	}
	
	void SetValue(Texture2D tex);
	
	void SetValue(RenderTarget2D rt);

	//void SetValue(RenderTarget2D *tar);
	//void SetDepthAsValue(RenderTarget2D *tar);
};
class Effect
{
	GLuint
    VertexShaderId,
    FragmentShaderId,
    GeomShaderId,
    ProgramId,
	WorldMatrix,
	ViewMatrix,
	NormalMatrix,
	LightPos,
	EyePos,
	ProjectionMatrix;
	char *PixelShader;
	char *VertexShader;
	char *GeomShader;
	const GLchar *PShader;
	const GLchar *VShader;
	const GLchar *GShader;
	map<string, EffectParameter*> *parameters;
	char *textFileRead(char *fn);
public:
	static string ShadersDirectory;
	Effect(string ShaderName);
	~Effect(void);
	void CreateShader();
	void CreateShaderInfo();
	void destroyShader();
	void Apply();
	//void AddParameter(aiMatrix4x4 param);
	//void AddWorld(glm::mat4x4 param);
	//void AddView(glm::mat4x4 param);
	//void AddProjection(glm::mat4x4 param);
	//void AddWorldNormal(glm::mat4x4 param);
	//void AddLightPos(glm::vec3 param);
	//void AddEye(glm::vec3 param);
	void glPrintError(GLenum err);
	EffectParameter* operator[](GLchar *name);
	EffectParameter* GetParameter(GLchar *name);
};