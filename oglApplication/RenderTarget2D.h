#pragma once
#include "stdafx.h"
#include "3rd\glew\include\GL\glew.h"


class RenderTarget2D
{
	GLuint frameBufferID;
	GLuint renderTargetTextureID;
	GLuint depthBufferID;
	GLenum textureComponents;
	GLenum textureFormat;
	GLenum textureType;
	GLenum depthFormat;
	int width;
	int height;
public:
	RenderTarget2D(void);
	RenderTarget2D(int Width, int Height, GLenum SurfaceComponents, GLenum SurfaceFormat, GLenum SurfaceType, GLenum DepthFormat);
	static void BindMRT(int num, RenderTarget2D rTargets[]);
	GLuint GetFrameBufferID();
	GLuint GetRenderTargetTextureID();
	GLuint GetDepthBufferID();
	void SetRenderTarget();
	void Bind();
	~RenderTarget2D(void);
};

