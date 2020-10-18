#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "3rd\GLFW\include\glfw3.h"
#include "Texture2D.h"
#include <iostream>
#include "3rd\glm\glm.hpp"

struct Material
{
	Texture2D* texture;
	glm::vec3 Color;
	float specularIntensity;
	float specularPower;
};

class SubModel
{
	Material material;
	glm::mat4x4 LocalWorld;
	GLuint
	    VaoId,
	    VertexVAOID,
		indexBuffer,
		vertexBuffer,
		colorBuffer,
		texCoordBuffer,
		normalBuffer,
	    ColorBufferId;
	glm::mat4x4 assimMat2Mymat(aiMatrix4x4 m);
	int indIter;
public:
	SubModel(aiMesh &mesh);
	void SetMaterial(Material m);
	Material &GetMaterial();
	void setLocalWorld(glm::mat4x4 world);
	glm::mat4x4 getLocalWorld();
	~SubModel(void);
	void Draw();
};

