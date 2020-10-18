#include "stdafx.h"
#include "RenderTarget2D.h"
#include <iostream>
using namespace std;


RenderTarget2D::RenderTarget2D(void)
{
}

RenderTarget2D::RenderTarget2D(int Width, int Height, GLenum InternalFotrmat, GLenum SurfaceFormat, GLenum SurfaceType, GLenum DepthFormat) :
width(Width), height(Height), textureComponents(InternalFotrmat), textureFormat(SurfaceFormat), textureType(SurfaceType), depthFormat(DepthFormat)
{

	//cout << "NEW MRT" << endl;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	glGenTextures(1, &renderTargetTextureID);
	glBindTexture(GL_TEXTURE_2D, renderTargetTextureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, textureComponents, width, height, 0, textureFormat, SurfaceType,0);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFotrmat, Width, Height, 0, SurfaceFormat, SurfaceType, 0);
//	glPrintError(glGetError());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTargetTextureID, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

}

void RenderTarget2D::BindMRT(int num, RenderTarget2D rTargets[])
{
	if (num <= 0)
		return;
	RenderTarget2D rtcheck;
	glBindFramebuffer(GL_FRAMEBUFFER, rTargets[0].frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rTargets[0].depthBufferID);

	GLenum *DrawBuffers = new GLenum[num];
	for (int i = 0; i < num; i++)
	{
		rtcheck = rTargets[i];
		DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rTargets[i].renderTargetTextureID, 0);
	}
	glDrawBuffers(num, DrawBuffers);
	glViewport(0, 0, 1280, 720);

}
GLuint RenderTarget2D::GetFrameBufferID(void)
{
	return frameBufferID;
}
GLuint RenderTarget2D::GetRenderTargetTextureID(void)
{
	return renderTargetTextureID;
}
GLuint RenderTarget2D::GetDepthBufferID(void)
{
	return depthBufferID;
}
void RenderTarget2D::Bind(void)
{
	glBindTexture(GL_TEXTURE_2D, this->renderTargetTextureID);
}
void RenderTarget2D::SetRenderTarget(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTargetTextureID, 0);
	glViewport(0, 0, this->width, this->height);
}
RenderTarget2D::~RenderTarget2D(void)
{
}
