#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <GLM\glm.hpp>
// Assimp library used for loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IResource.h"

/**
* A mesh resource contains all the data in a mesh resource file including 
* the vertices, normals, texture coordinates and indices which are stored in data vectors.
*/

struct MeshObj
{
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_textureCoords;
	std::vector<glm::vec3> m_materialIndex;
	std::vector<unsigned int> m_indices; // To create the faces
};

class Mesh : public IResource
{
public:
	Mesh() { }
	~Mesh() { }

	Mesh(std::string filePath) : m_resourceType(RESOURCE_MESH), m_filePath(filePath)
	{
		CreateMeshFromFile(filePath);
	}

	// Function uses assimp
	std::unique_ptr<IResource> CreateMeshFromFile(std::string filePath);

	// Helper functions to load mesh
	void LoadVertices(aiMesh* meshObj);
	void LoadNormals(aiMesh* meshObj);
	void LoadTextureCoords(aiMesh* meshObj);
	void LoadFaces(aiMesh* meshObj);

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
	unsigned int m_resourceId;
	std::string m_filePath;
	RESOURCE_TYPE m_resourceType;
	bool m_loaded;

	std::vector<struct MeshObj> m_meshBatch;
};