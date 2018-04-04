#include "Mesh.h"

std::unique_ptr<IResource> Mesh::CreateMeshFromFile(std::string filePath)
{
	Assimp::Importer importer;

	const aiScene* scene =
		importer.ReadFile(filePath.c_str(), aiProcess_Triangulate);

	if (!scene)
		std::cout << "Failed to load model: " << filePath << std::endl;

	int meshCount = scene->mNumMeshes;
	std::unique_ptr<Mesh> meshBatch(new Mesh);
	
	// Selects only the first mesh
	meshBatch->LoadVertices(scene->mMeshes[0]);
	meshBatch->LoadNormals(scene->mMeshes[0]);
	meshBatch->LoadTextureCoords(scene->mMeshes[0]);
	meshBatch->LoadFaces(scene->mMeshes[0]);

	importer.FreeScene();

	std::unique_ptr<IResource> meshResource = std::move(meshBatch);

	return meshResource;
}

void Mesh::LoadVertices(aiMesh* meshObj)
{

}

void Mesh::LoadNormals(aiMesh* meshObj)
{

}

void Mesh::LoadTextureCoords(aiMesh* meshObj)
{

}

void Mesh::LoadFaces(aiMesh* meshObj)
{

}

const std::string& Mesh::GetFilePath() const
{
	return m_filePath;
}

void Mesh::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}

const unsigned int& Mesh::GetResourceId() const
{
	return m_resourceId;
}

void Mesh::SetResourceId(unsigned int id)
{
	m_resourceId = id;
}

const RESOURCE_TYPE& Mesh::GetResourceType() const
{
	return m_resourceType;
}

void Mesh::SetResourceType(RESOURCE_TYPE type)
{
	m_resourceType = type;
}

bool Mesh::IsLoaded() const
{
	return m_loaded;
}

void Mesh::SetLoaded(bool value)
{
	m_loaded = value;
}
