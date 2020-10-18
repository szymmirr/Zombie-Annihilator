#include "Model.h"


Model::Model(std::vector<SubModel*> subModels)
{
	meshes = subModels;
	elements = meshes.size();
}


Model::~Model(void)
{
}

void Model::Draw()
{
	for(int i = 0; i < (int)meshes.size(); i++)
	{
		meshes.at(i)->Draw();
	}
}
void Model::DrawMesh(int i)
{
		meshes.at(i)->Draw();
}
SubModel* Model::GetMesh(int i)
{
		return meshes.at(i);
}
int Model::Elements()
{
	return elements;
}