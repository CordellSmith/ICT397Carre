// Assimp implementation guided by https://learnopengl.com/Model-Loading/Assimp
#include "MeshResource.h"

IResource* MeshResource::CreateMeshFromFile(std::string filePath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return 0;
	}

	// Check guide for this function... not implemented but taken code so not traversing through a linked list (scene node)
	//processNode(scene->mRootNode, scene);

	MeshResource* meshBatch = new MeshResource(scene->mNumMeshes);

	// Loop through all meshs in the scene if there is any and load data into m_meshBatch
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		meshBatch->LoadVertices(i, scene->mMeshes[i]);
		meshBatch->LoadNormals(i, scene->mMeshes[i]);
		meshBatch->LoadTextureCoords(i, scene->mMeshes[i]);
		meshBatch->LoadFaces(i, scene->mMeshes[i]);
	}
	
	import.FreeScene();

	IResource* meshResource = meshBatch;

	return meshResource;
}

void MeshResource::LoadVertices(unsigned int meshIndex, aiMesh* meshObj)
{
	// CONTINUE FROM HERE!
	int vertexCount = meshObj->mNumVertices;
	m_meshBatch[meshIndex].m_position.reserve(vertexCount);

	for (int i = 0; i < vertexCount; i++)
	{
		glm::vec3 vertexPos;
		vertexPos.x = meshObj->mVertices[i].x;
		vertexPos.y = meshObj->mVertices[i].y;
		vertexPos.z = meshObj->mVertices[i].z;
		// In mesh object i, it is storing all the vertex data in the MeshObj.m_vertices array of vec3
		m_meshBatch[meshIndex].m_position.push_back(vertexPos);
	}
}

void MeshResource::LoadNormals(unsigned int meshIndex, aiMesh* meshObj)
{
	int vertexCount = meshObj->mNumVertices;
	m_meshBatch[meshIndex].m_normal.resize(vertexCount);

	for (int i = 0; i < vertexCount; i++)
	{
		glm::vec3 normalCoord;
		normalCoord.x = meshObj->mNormals[i].x;
		normalCoord.y = meshObj->mNormals[i].y;
		normalCoord.z = meshObj->mNormals[i].z;
		// In mesh object i, it is storing all the normal coordinates in the MeshObj.m_normals array of vec3
		m_meshBatch[meshIndex].m_normal.push_back(normalCoord);
	}
}

void MeshResource::LoadTextureCoords(unsigned int meshIndex, aiMesh* meshObj)
{
	int vertexCount = meshObj->mNumVertices;
	// Check to see if the does the mesh contain texture coordinates
	if (meshObj->mTextureCoords[0])
	{
		for (int i = 0; i < vertexCount; i++)
		{
			glm::vec2 texCoord;
			texCoord.x = meshObj->mTextureCoords[0][i].x;
			texCoord.y = meshObj->mTextureCoords[0][i].y;
			// In mesh object i, it is storing all the texture coordinates in the MeshObj.m_normals array of vec2
			m_meshBatch[meshIndex].m_texCoord.push_back(texCoord);
		}
	}
	else
	{
		std::cout << "No Texture Coordinates Found!" << std::endl;
		// Sets the default texture coordinate to (0.0, 0.0)
		m_meshBatch[meshIndex].m_texCoord.push_back(glm::vec2(0.0f, 0.0f));
		return;
	}
}

void MeshResource::LoadFaces(unsigned int meshIndex, aiMesh* meshObj)
{
	for (unsigned int i = 0; i < meshObj->mNumFaces; i++)
	{
		aiFace face = meshObj->mFaces[i];
		// After this outer loop has finished we now have a complete set of vertices and index data for drawing the mesh via glDrawElements
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			m_meshBatch[meshIndex].m_indices.push_back(face.mIndices[j]);
	}
}

// Private Functions
/*
void MeshResource::SetupMesh()
{
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);

// This needs to be looked at! Check guide for reference
glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

// vertex positions
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
// vertex normals
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
// vertex texture coords
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

glBindVertexArray(0);
}

*/

void MeshResource::UnloadMesh()
{
	for (int i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].m_position.clear();
		m_meshBatch[i].m_normal.clear();
		m_meshBatch[i].m_texCoord.clear();
		m_meshBatch[i].m_indices.clear();
	}
}


// Inherited functions - Getters and Setters
const std::string& MeshResource::GetFilePath() const
{
	return m_filePath;
}

void MeshResource::SetFilePath(const std::string & filePath)
{
	m_filePath = filePath;
}

const unsigned int& MeshResource::GetResourceId() const
{
	return m_resourceId;
}

void MeshResource::SetResourceId(unsigned int id)
{
	m_resourceId = id;
}

const RESOURCE_TYPE& MeshResource::GetResourceType() const
{
	return m_resourceType;
}

void MeshResource::SetResourceType(RESOURCE_TYPE type)
{
	m_resourceType = type;
}

bool MeshResource::IsLoaded() const
{
	return m_loaded;
}

void MeshResource::SetLoaded(bool value)
{
	m_loaded = value;
}
