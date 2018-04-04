#pragma once

#include <iostream>
#include <vector>
#include <GLM\glm.hpp>

#include "Resource.h"

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
	std::vector<unsigned int> m_indices; // To create faces
};

class Mesh : public Resource
{
public:
	Mesh() { }
	~Mesh() { }

	Mesh(unsigned int resourceId) : m_resourceId(resourceId) { }

	virtual const std::string& GetFilePath() const;
	virtual void SetFilePath(const std::string & filePath);

protected:
	unsigned int m_resourceId;
	std::string m_filePath;

	std::vector<struct MeshObj> m_meshBatch;
};