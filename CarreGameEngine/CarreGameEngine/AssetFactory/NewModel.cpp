#include "NewModel.h"

void NewModel::LoadModel(std::string filePath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	else
		ProcessNode(scene->mRootNode, scene);
}

void NewModel::ProcessNode(aiNode* node, const aiScene* scene)
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

Mesh NewModel::ProcessMesh(aiMesh *mesh, const aiScene *scene)
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

	return Mesh(vertices, indices);
}

void NewModel::Draw()
{
	for (unsigned int i = 0; i < m_meshBatch.size(); i++)
		m_meshBatch[i].Draw();
}