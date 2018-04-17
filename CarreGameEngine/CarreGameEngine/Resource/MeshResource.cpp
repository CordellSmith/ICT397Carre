// Assimp implementation guided by https://learnopengl.com/Model-Loading/Assimp
#include "MeshResource.h"

MeshResource::MeshResource() : m_resourceType(RESOURCE_MESH)
{
	SetResourceId(rand() & 100);
}

IResource* MeshResource::CreateMeshFromFile(const std::string filePath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return 0;
	}

	ProcessNode(scene->mRootNode, scene);

	return this;
}

void MeshResource::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshBatch.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

SingleMesh MeshResource::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex3> vertices;
	std::vector<unsigned int> indices;
	//std::vector<Texture> textures;

	// process vertex positions, normals and texture coordinates
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex3 vertex;
		glm::vec3 vertexPos;
		glm::vec3 normalCoord;
		glm::vec2 texCoord;

		vertexPos.x = mesh->mVertices[i].x;
		vertexPos.y = mesh->mVertices[i].y;
		vertexPos.z = mesh->mVertices[i].z;

		normalCoord.x = mesh->mNormals[i].x;
		normalCoord.y = mesh->mNormals[i].y;
		normalCoord.z = mesh->mNormals[i].z;

		texCoord.x = mesh->mTextureCoords[0][i].x;
		texCoord.y = mesh->mTextureCoords[0][i].y;

		vertex.m_position = vertexPos;
		vertex.m_normal = normalCoord;
		vertex.m_texCoords = texCoord;

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return SingleMesh(vertices, indices);
}

/*
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
*/

// Private Functions
const void MeshResource::Load()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// This needs to be looked at! Check guide for reference
	glBufferData(GL_ARRAY_BUFFER, m_singleMesh.m_vertices.size() * sizeof(Vertex3), &m_singleMesh.m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_singleMesh.m_indices.size() * sizeof(unsigned int), &m_singleMesh.m_indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, m_normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, m_texCoords));

	glBindVertexArray(0);
	m_loaded = true;
}

void MeshResource::Draw(Shader shader)
{
	/*
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		//shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	*/
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_singleMesh.m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

const void MeshResource::Unload()
{
	m_meshBatch.clear();
	m_loaded = false;

	//for (int i = 0; i < m_meshBatch.size(); i++)
	//{
	//	m_meshBatch[i].m_position.clear();
	//	m_meshBatch[i].m_normal.clear();
	//	m_meshBatch[i].m_texCoord.clear();
	//	m_meshBatch[i].m_indices.clear();
	//}
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
