#pragma once
#include <vector>
#include "SubModel.h"

class Model
{
	
	std::vector<SubModel*> meshes;
	int elements;
public:
	Model(std::vector<SubModel*>);
	
	void DrawMesh(int i);
	SubModel* GetMesh(int i);
	int  Elements();//gets number of elements
	~Model(void);
	void Draw();
};

