#include "Model.h"

#include "..\ImageDB\stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

void Model::LoadModel(std::string filePath)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	m_directory = filePath.substr(0, filePath.find_last_of('/'));

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	}
	else
		ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
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

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex3> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// process mesh data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex3 vertex;
		glm::vec3 vertexPos;
		glm::vec4 colour;
		glm::vec3 normalCoord;
		glm::vec2 texCoord;
		glm::vec3 tangent;
		glm::vec3 biTangent;
		
		// vertex positions
		vertexPos.x = mesh->mVertices[i].x;
		vertexPos.y = mesh->mVertices[i].y;
		vertexPos.z = mesh->mVertices[i].z;
		vertex.m_position = vertexPos;
		// colours (randomised)
		colour = glm::vec4(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), 0.5f);
		vertex.m_colour = colour;
		// normals
		if (mesh->HasNormals())
		{
			normalCoord.x = mesh->mNormals[i].x;
			normalCoord.y = mesh->mNormals[i].y;
			normalCoord.z = mesh->mNormals[i].z;
			vertex.m_normal = normalCoord;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = texCoord;
		}
		else
			vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
		// tangents and bitangents
		if (mesh->HasTangentsAndBitangents())
		{
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			vertex.m_tangent = tangent;

			biTangent.x = mesh->mBitangents[i].x;
			biTangent.y = mesh->mBitangents[i].y;
			biTangent.z = mesh->mBitangents[i].z;
			vertex.m_biTangent = biTangent;
		}

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (std::strcmp(m_texturesLoaded[j].m_path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.m_id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.m_type = typeName;
			texture.m_path = str.C_Str();
			textures.push_back(texture);
			m_texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void Model::Prepare(std::string vertShader, std::string fragShader)
{
	m_shader->Initialize(vertShader, fragShader);
}

void Model::Draw()
{
	for (unsigned int i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].Draw(m_shader);
	}
}

void Model::Destroy()
{
	// Todo: implement destroy function of all meshes of the model
	return void();
}


void Model::SetPosition(glm::vec3 position)
{
	m_position = position;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetPosition(position);
	}
}

void Model::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetScale(scale);
	}
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filePath = std::string(path);
	filePath = directory + '/' + filePath;

	return TextureManager::Instance().LoadTexture(filePath);
}

const void Model::SetCamera(Camera* camera) 
{
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetCamera(camera);
	}
}