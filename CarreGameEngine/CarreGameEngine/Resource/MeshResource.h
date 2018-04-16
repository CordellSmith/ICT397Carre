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

#include "IResource.h"

/**
* A mesh resource contains all the data in a mesh resource file including 
* the vertices, normals, texture coordinates and indices which are stored in data vectors.
*/

struct SingleMesh
{
	std::vector<glm::vec3> m_position;
	std::vector<glm::vec3> m_normal;
	std::vector<glm::vec2> m_texCoord;
	std::vector<unsigned int> m_indices; // To create the faces
	// Not implemented yet
	//std::vector<glm::vec3> m_materialIndex;
};

class MeshResource : public IResource
{
public:
	MeshResource() { }
	~MeshResource() { UnloadMesh(); }

	MeshResource(unsigned int meshCount) : m_resourceType(RESOURCE_MESH), m_meshCount(meshCount), m_resourceId(100) { m_meshBatch.resize(meshCount); }
	MeshResource(std::string filePath) : m_resourceType(RESOURCE_MESH), m_filePath(filePath) { }

	// Function uses assimp
	IResource* CreateMeshFromFile(std::string filePath);

	// Helper functions to load mesh
	void LoadVertices(unsigned int meshIndex, aiMesh* meshObj);
	void LoadNormals(unsigned int meshIndex, aiMesh* meshObj);
	void LoadTextureCoords(unsigned int meshIndex, aiMesh* meshObj);
	void LoadFaces(unsigned int meshIndex, aiMesh* meshObj);

	int GetMeshCount() const { return m_meshCount; }

	// This function will be called in the Resource Factory
	//void Draw(Shader shader)
	//{
	//	for (unsigned int i = 0; i < meshes.size(); i++)
	//		meshes[i].Draw(shader);
	//}

	// Inherited from IResource interface class
	virtual const std::string& GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);
	virtual const unsigned int& GetResourceId() const;
	virtual void SetResourceId(unsigned int id);
	virtual const RESOURCE_TYPE& GetResourceType() const;
	virtual void SetResourceType(RESOURCE_TYPE type);
	virtual bool IsLoaded() const;
	virtual void SetLoaded(bool value);

protected:
	std::vector<SingleMesh> m_meshBatch;
	int m_meshCount;

	unsigned int m_resourceId;
	std::string m_filePath;
	RESOURCE_TYPE m_resourceType;
	bool m_loaded;

	unsigned int VAO, VBO, EBO;
	//void SetupMesh();

	// Todo: deallocate m_meshObj
	void UnloadMesh();
};
