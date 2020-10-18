
#include "3rd\glew\include\GL\glew.h"
#include "SubModel.h"
#include "3rd\GLFW\include\glfw3.h"
using namespace std;

SubModel::SubModel(aiMesh &mesh)
{
	int vertsCount = mesh.mNumVertices;
	GLfloat* vertices = new GLfloat[vertsCount * 3];
	GLfloat *colors;
	indIter = 0;
	GLfloat *TexCoords;
	GLfloat *normals;
	LocalWorld = glm::mat4x4();
	if(mesh.HasBones())
	{
		if(mesh.mNumBones == 1)
		{
			 LocalWorld = assimMat2Mymat( mesh.mBones[0]->mOffsetMatrix);
		}
	}
		GLuint* indices = new GLuint[vertsCount];
	//memory stuff
		colors = new GLfloat[vertsCount * 4];

		TexCoords = new GLfloat[vertsCount * 2];

		normals = new GLfloat[vertsCount * 3];
		
			if(!mesh.HasNormals())
			{
				cout << "WARNING: mesh does not contain normal vectors.\n";
			}
		//vertsCount*=4.0f;
		for(int i = 0; i<vertsCount; i++)
		{
			vertices[i*3]   = mesh.mVertices[i].x;
			vertices[i*3+1] = mesh.mVertices[i].y;
			vertices[i*3+2] = mesh.mVertices[i].z;
			if(i < (int)mesh.mNumFaces)
			{
				for(int za = 0; za < (int)mesh.mFaces->mNumIndices; za++)
				{
					indices[indIter]   = mesh.mFaces[i].mIndices[za];
					indIter++;
				}
			}
			if(false)
			{

				colors[i*4]   = mesh.mColors[i]->r;
				colors[i*4+1] = mesh.mColors[i]->g;
				colors[i*4+2] = mesh.mColors[i]->b;
				colors[i*4+3] = mesh.mColors[i]->a;
			
			}
			else
			{
				colors[i*4]   = 1.0f;
				colors[i*4+1] = 0.0f;
				colors[i*4+2] = 0.0f;
				colors[i*4+3] = 0.0f;

			}
			if(mesh.HasTextureCoords(0))
			{
				TexCoords[i*2]   = mesh.mTextureCoords[0][i].x;
				TexCoords[i*2+1] = mesh.mTextureCoords[0][i].y;
			}
			else
			{
				TexCoords[i*2]   = 0;
				TexCoords[i*2+1] = 0;
			}
			
			if(mesh.HasNormals())
			{
				normals[i*3]   = mesh.mNormals[i].x;
				normals[i*3+1] = mesh.mNormals[i].y;
				normals[i*3+2] = mesh.mNormals[i].z;
			}
			else
			{
				normals[i*3]   = 0;
				normals[i*3+1] = 1;
				normals[i*3+2] = 0;
			}
		}

		VertexVAOID = 0;
		glGenVertexArrays(1, &VertexVAOID);

		glBindVertexArray(VertexVAOID);
		
		//vertices
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* vertsCount * 3, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//indices
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indIter, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//colors
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertsCount * 4, colors, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//texcoords
		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertsCount * 2, TexCoords, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//normals
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertsCount * 3, normals, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		

		
		glBindVertexArray(0);


		//delete [] vertices;
		delete [] colors;
		delete [] indices;
		delete [] TexCoords;
		delete [] normals;	
		//cout << mesh.mNumVertices << endl;
		//cout << indIter << endl;
		int indCount = indIter;
		vertsCount = vertsCount;
}

SubModel::~SubModel(void)
{
}

glm::mat4x4 SubModel::assimMat2Mymat(aiMatrix4x4 m)
{
	glm::mat4x4 mat(1.0f);
	mat[0] = glm::vec4(m.a1, m.b1, m.c1, m.d1);
	mat[1] = glm::vec4(m.a2, m.b2, m.c2, m.d2);
	mat[2] = glm::vec4(m.a3, m.b3, m.c3, m.d3);
	mat[3] = glm::vec4(m.a4, m.b4, m.c4, m.d4);
	return mat;
}

void SubModel::SetMaterial(Material m)
{
	material = m;
}
void SubModel::setLocalWorld(glm::mat4x4 world)
{
	LocalWorld = world;
}	
glm::mat4x4 SubModel::getLocalWorld()
{
	return LocalWorld;
}	
void SubModel::Draw()
{
	glBindVertexArray(VertexVAOID);
	glDrawArrays(GL_TRIANGLES, 0, indIter);
	glBindVertexArray(0);
}

Material &SubModel::GetMaterial()
{
	return material;
}