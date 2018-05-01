#include "NewModel.h"

#include "..\ImageDB\stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

void NewModel::LoadModel(std::string filePath)
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
	int numOfVertexs = 0;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// process vertex positions, normals and texture coordinates
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex3 vertex;
		glm::vec3 vertexPos;
		glm::vec4 colour;
		glm::vec3 normalCoord;
		glm::vec2 texCoord;

		vertexPos.x = mesh->mVertices[i].x;
		vertexPos.y = mesh->mVertices[i].y;
		vertexPos.z = mesh->mVertices[i].z;
		vertex.m_position = vertexPos;

		colour = glm::vec4(((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), ((float)rand() / (RAND_MAX)), 1.0f);
		vertex.m_colour = colour;

		normalCoord.x = mesh->mNormals[i].x;
		normalCoord.y = mesh->mNormals[i].y;
		normalCoord.z = mesh->mNormals[i].z;
		vertex.m_normal = normalCoord;

		if (mesh->mTextureCoords[0])
		{
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = texCoord;
		}
		else
			vertex.m_texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
		numOfVertexs++;
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

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, numOfVertexs, indices, textures);
}

std::vector<Texture> NewModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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

void NewModel::Prepare(std::string vertShader, std::string fragShader)
{
	m_shader->Initialize(vertShader, fragShader);
}

void NewModel::Draw()
{
	for (unsigned int i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].Draw(m_shader);
	}
}

void NewModel::Destroy()
{
	// Todo: implement destroy function of all meshes of the model
	return void();
}


void NewModel::SetPosition(glm::vec3 position)
{
	m_position = position;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetPosition(position);
	}
}

void NewModel::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetScale(scale);
	}
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

const void NewModel::SetCamera(Camera* camera) 
{
	for (size_t i = 0; i < m_meshBatch.size(); i++)
	{
		m_meshBatch[i].SetCamera(camera);
	}
}