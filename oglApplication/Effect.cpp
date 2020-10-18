#include "3rd\glew\include\GL\glew.h"
#include "Effect.h"
#include "stdafx.h"
#include "Texture2D.h"
#include <iostream>
#include <map>
#include <vector>
#include "RenderTarget2D.h"
using namespace std;

char *Effect::textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

float *MatrixToFloat(aiMatrix4x4 param)
{
	float *mat = new float[16];
	mat[0]  = param.a1;
	mat[1]  = param.a2;
	mat[2]  = param.a3;
	mat[3]  = param.a4;
	mat[4]  = param.b1;
	mat[5]  = param.b2;
	mat[6]  = param.b3;
	mat[7]  = param.b4;
	mat[8]  = param.c1;
	mat[9]  = param.c2;
	mat[10] = param.c3;
	mat[11] = param.c4;
	mat[12] = param.d1;
	mat[13] = param.d2;
	mat[14] = param.d3;
	mat[15] = param.d4;
	return mat;
}

Effect::Effect(string ShaderName)
{
	string psDir = ShaderName;
	PixelShader = textFileRead((char*)(psDir + ".ps").c_str());
	VertexShader = textFileRead((char*)(psDir + ".vs").c_str());
	textFileRead((char*)(psDir + ".gs").c_str());
}

void Effect::CreateShader()
{
	GLenum ErrorCheckValue = glGetError();
	const GLchar *v_str = VertexShader;
	const GLchar *f_str = PixelShader;

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShaderId, 1, &v_str, NULL);
	glShaderSource(FragmentShaderId, 1, &f_str, NULL);
	glCompileShader(VertexShaderId);
	glPrintError(glGetError());
	glCompileShader(FragmentShaderId);
	glPrintError(glGetError());
    ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glPrintError(glGetError());
	glAttachShader(ProgramId, FragmentShaderId);
	glPrintError(glGetError());

    glLinkProgram(ProgramId);
	glPrintError(glGetError());
	int ucount = 0;
	glGetProgramiv(ProgramId, GL_ACTIVE_UNIFORMS, &ucount);

	GLchar buf[256];
	GLsizei u_length;
	GLsizei u_size;
	GLuint u_textureid = 0;
	GLenum u_type;
	parameters = new map<string, EffectParameter*>();
	for(int i  = 0; i<ucount; i++)
	{
		glGetActiveUniform(ProgramId, i, 256, &u_length, &u_size, &u_type, buf);
		if(u_type == GL_SAMPLER_2D)
		{
			string u_name(buf);
			(*parameters)[u_name] = new EffectParameter(ProgramId, buf, u_type, u_textureid);
			u_textureid++;
		}
		else
		{
			string u_name(buf);
			EffectParameter* p = new EffectParameter(ProgramId, buf, u_type);;
			(*parameters)[u_name] = p;
		}

	}
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create the shaders: %s \n",
			glewGetErrorString(ErrorCheckValue)
        );
        exit(-1);
    }
}


void Effect::CreateShaderInfo()
{
	GLenum ErrorCheckValue = glGetError();
	const GLchar *v_str = VertexShader;
	const GLchar *f_str = PixelShader;

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShaderId, 1, &v_str, NULL);
	glShaderSource(FragmentShaderId, 1, &f_str, NULL);
	glCompileShader(VertexShaderId);
	glPrintError(glGetError());
	
	GLint isCompiled = 0;
	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
	 
		char* log = new char[maxLength];
		glGetShaderInfoLog(FragmentShaderId, maxLength, &maxLength, &log[0]);
		//cout << log;

		glDeleteShader(VertexShaderId);
		exit(-1);
	}
	glCompileShader(FragmentShaderId);
	glPrintError(glGetError());
	
	glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
	 
		char* log = new char[maxLength];
		glGetShaderInfoLog(FragmentShaderId, maxLength, &maxLength, &log[0]);
		//cout << log;

		glDeleteShader(FragmentShaderId);
		exit(-1);
	}
    ProgramId = glCreateProgram();
	glPrintError(glGetError());
	glAttachShader(ProgramId, VertexShaderId);
	glPrintError(glGetError());
	glAttachShader(ProgramId, FragmentShaderId);
	glPrintError(glGetError());

    glLinkProgram(ProgramId);
	glPrintError(glGetError());
	
	glGetProgramiv(ProgramId, GL_LINK_STATUS, &isCompiled);
	glPrintError(glGetError());
	if(isCompiled == GL_FALSE)
	{
		cout << "Error while linking the shader.\n";
		exit(-1);
	}
	int ucount = 0;
	glGetProgramiv(ProgramId, GL_ACTIVE_UNIFORMS, &ucount);
	//cout << "Atributes count: " << ucount << endl;
	GLchar buf[256];
	GLsizei u_length;
	GLsizei u_size;
	GLuint u_textureid = 0;
	GLenum u_type;
	parameters = new map<string, EffectParameter*>();
	for(int i  = 0; i<ucount; i++)
	{
		glGetActiveUniform(ProgramId, i, 256, &u_length, &u_size, &u_type, buf);
		cout << "atribute "     << i << ": \n";
		cout << "\tname: "		<< buf << endl;
		cout << "\ttype: "		<< u_type << endl;
		cout << "\tlength: "	<< u_length << endl;
		cout << "\tsize: "		<< u_size << endl;
		if(u_type == GL_SAMPLER_2D)
		{
			
			string u_name(buf);
			(*parameters)[u_name] = new EffectParameter(ProgramId, buf, u_type, u_textureid);
			u_textureid++;
		}
		else
		{
			string u_name(buf);
			EffectParameter* p = new EffectParameter(ProgramId, buf, u_type);;
			(*parameters)[u_name] = p;
		}
		//GLchar *u_name = new GLchar[u_size];

	}
	//WorldMatrix = glGetUniformLocation(ProgramId, "World");
	//ViewMatrix = glGetUniformLocation(ProgramId, "View");
	//ProjectionMatrix = glGetUniformLocation(ProgramId, "Projection");
	//NormalMatrix = glGetUniformLocation(ProgramId, "WorldNormal");
	//LightPos = glGetUniformLocation(ProgramId, "LightPos");
	//EyePos = glGetUniformLocation(ProgramId, "EyePos");
    ErrorCheckValue = glGetError();
    if (ErrorCheckValue != GL_NO_ERROR)
    {
        fprintf(
            stderr,
            "ERROR: Could not create the shaders: %s \n",
			glewGetErrorString(ErrorCheckValue)
        );
        exit(-1);
    }
}

void Effect::Apply(void)
{
    GLenum ErrorCheckValue = glGetError();
    glUseProgram(ProgramId);
    ErrorCheckValue = glGetError();
   if (ErrorCheckValue != GL_NO_ERROR)
   {
       fprintf(
           stderr,
           "ERROR: glUseProgram: %s  %d\n",
		   glewGetErrorString(ErrorCheckValue),
			ProgramId
       );
       exit(-1);
   }
}

Effect::~Effect(void)
{
}

//void Effect::AddParameter(aiMatrix4x4 param)
//{
//	glUniformMatrix4fv(WorldMatrix, 1, GL_FALSE, MatrixToFloat(param));
//}
//
//void Effect::AddWorld(glm::mat4x4 param)
//{
//	glUniformMatrix4fv(WorldMatrix, 1, GL_FALSE, glm::value_ptr(param));
//}
//
//void Effect::AddView(glm::mat4x4 param)
//{
//	glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(param));
//}
//
//void Effect::AddProjection(glm::mat4x4 param)
//{
//	glUniformMatrix4fv(ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(param));
//}
//
//void Effect::AddWorldNormal(glm::mat4x4 param)
//{
//	glUniformMatrix4fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(param));
//}
//
//void Effect::AddLightPos(glm::vec3 param)
//{
//	glUniform3fv(LightPos, 1, glm::value_ptr(param));
//}
//
//void Effect::AddEye(glm::vec3 param)
//{
//	glUniform3fv(EyePos, 1, glm::value_ptr(param));
//}

EffectParameter* Effect::operator[](GLchar *name)
{
	return (*parameters)[name];
}

EffectParameter* Effect::GetParameter(GLchar *name)
{
	bool test = parameters->find(name) != parameters->end();
	if (test)
		return (*parameters)[name];
	else
		return NULL;
}

void EffectParameter::SetValue(Texture2D tex)
{
	if(this == NULL)
	{
		//cout << "binding texture to inactive uniform\n";
		return;
	}
	glActiveTexture(GL_TEXTURE0 + textureID);
	tex.Bind();
	glUniform1i(paramID, textureID);
}

void Effect::glPrintError(GLenum err)
{
	const GLubyte *errString;

	if (err != GL_NO_ERROR)
	{
		errString = glewGetErrorString(err);
	   fprintf (stderr, "OpenGL Error: %s\n", errString);
	}
}

void EffectParameter::SetValue(RenderTarget2D rt)
{
	if (this == NULL)
	{
		//cout << "binding texture to inactive uniform\n";
		return;
	}
	glActiveTexture(GL_TEXTURE0 + textureID);
	rt.Bind();
	glUniform1i(paramID, textureID);
	//glPrintError(glGetError());
	int y = 1;
	//	glPrintError(glGetError());
}

string Effect::ShadersDirectory = "shaders";