#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <GLM\glm.hpp>
// Assimp library used for loading
// Guided by https://learnopengl.com/Model-Loading/Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL\glew.h>
#include <cstdlib>

#include "IResource.h"
#include "..\headers\Shader.h"

/**
* A mesh resource contains all the data in a mesh resource file including 
* the vertices, normals, texture coordinates and indices which are stored in data vectors.
*/

struct Vertex3
{
	glm::vec3 m_position;											// The x,y,z position
	glm::vec3 m_normal;										// The normal coordinates
	glm::vec2 m_texCoords;											// The uv coordinates
};

struct SingleMesh
{
	SingleMesh() { }
	SingleMesh(std::vector<Vertex3> vertices, std::vector<unsigned int> indices)
	{
		m_vertices = vertices;
		m_indices = indices;
	}

	std::vector<Vertex3> m_vertices;
	std::vector<unsigned int> m_indices; // To create the faces
	//std::vector<Texture> m_textures;
};

class MeshResource : public IResource
{
public:
	MeshResource();
	~MeshResource() { Unload(); }
	MeshResource(std::string filePath) : m_resourceType(RESOURCE_MESH), m_filePath(filePath) { }

	// Function uses assimp
	IResource* CreateMeshFromFile(const std::string filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	SingleMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	// Helper functions to load mesh
	//void LoadVertices(unsigned int meshIndex, aiMesh* meshObj);
	//void LoadNormals(unsigned int meshIndex, aiMesh* meshObj);
	//void LoadTextureCoords(unsigned int meshIndex, aiMesh* meshObj);
	//void LoadFaces(unsigned int meshIndex, aiMesh* meshObj);

	int GetMeshCount() const { return m_meshCount; }

	// Inherited from IResource interface class
	virtual const void Load();
	virtual const void Unload();
	virtual const std::string& GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);
	virtual const unsigned int& GetResourceId() const;
	virtual void SetResourceId(unsigned int id);
	virtual const RESOURCE_TYPE& GetResourceType() const;
	virtual void SetResourceType(RESOURCE_TYPE type);
	virtual bool IsLoaded() const;
	virtual void SetLoaded(bool value);

protected:
	SingleMesh m_singleMesh;
	std::vector<SingleMesh> m_meshBatch;
	int m_meshCount;

	unsigned int m_resourceId;
	std::string m_filePath;
	RESOURCE_TYPE m_resourceType;
	bool m_loaded;

	unsigned int VAO, VBO, EBO;
	void Draw(Shader shader);

	// Todo: deallocate m_meshObj
	//void UnloadMesh();
};
