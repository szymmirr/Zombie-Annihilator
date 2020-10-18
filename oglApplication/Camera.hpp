#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include "stdafx.h"
#include "3rd\glm\glm.hpp"
#include "3rd\glm\gtc\matrix_transform.hpp"

class Camera
{

	glm::vec3 up;
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::mat4x4 View;
	glm::mat4x4 Projection;
	float width;
	float height;

	public:
		Camera(){}

	Camera(float Width, float Height)
		:width(Width), height(Height), position(glm::vec3(0,0.5f,-2.0f)), lookAt(glm::vec3(0.0f,0.0f,0.0f))
	{
		width = Width;
		height = Height;
		up = glm::vec3(0,1,0);
		View = glm::lookAt(position, lookAt, up );
		Projection = glm::perspective(45.0f,Width/Height,0.1f, 100.0f);
	}
	Camera(glm::vec3 Position, glm::vec3 Target, float Width, float Height)
		:width(Width), height(Height), position(Position), lookAt(Target)
	{
		width = Width;
		height = Height;
		up = glm::vec3(0,1,0);
		View = glm::lookAt(position, lookAt, up);
		Projection = glm::perspective(45.0f,Width/Height,0.1f, 100.0f);
	}

	glm::mat4x4 getView()
	{
		View = glm::lookAt(position, lookAt, up);
		return View;
	}

	glm::mat4x4 getProjection()
	{
		return Projection;
	}

	glm::mat4x4 getViewProjection()
	{
		View = glm::lookAt(position, lookAt, up);
		return Projection*View;
	}

	void setPosition(glm::vec3 Position)
	{
		position = Position;
	}

	void setTarget(glm::vec3 Target)
	{
		lookAt = Target;
	}
	
	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec3 getTarget()
	{
		return lookAt;
	}

	void setFOV(float fov)
	{
		Projection = glm::perspective(fov,width/height,0.1f, 100.0f);
	}

	void setUp(glm::vec3 upv)
	{
		up = upv;
	}

};

#endif

