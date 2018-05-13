#include "Terrain.h"

Terrain::Terrain(std::string assetName, float heightScale, float blockScale)
	: m_GLVertexBuffer(0)
	, m_GLNormalBuffer(0)
	, m_GLColorBuffer(0)
	, m_GLTex0Buffer(0)
	, m_GLTex1Buffer(0)
	, m_GLTex2Buffer(0)
	, m_GLIndexBuffer(0)
	, m_LocalToWorldMatrix(1)
	, m_InverseLocalToWorldMatrix(1)
	, m_HeightmapDimensions(0, 0)
	, m_fHeightScale(heightScale)
	, m_fBlockScale(blockScale)
{
	m_assetType = ASS_TERRAIN;
	m_assetName = assetName;
	memset(m_GLTextures, 0, sizeof(m_GLTextures));
}

Terrain::Terrain(std::string textureFilePath, std::string heightmapFilePath)
	: m_textureFilePath(textureFilePath),
	m_heightmapFilePath(heightmapFilePath)
{
	m_assetType = ASS_TERRAIN;
}

bool Terrain::LoadTexture(unsigned int textureNo)
{
	assert(textureNo < m_uiNumTextures);
	DeleteTexture(m_GLTextures[textureNo]);

	m_GLTextures[textureNo] = SOIL_load_OGL_texture(m_textureFilePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (m_GLTextures[textureNo] != 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_GLTextures[textureNo]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return (m_GLTextures[textureNo] != 0);
}

bool Terrain::LoadHeightmap(unsigned char bitsPerPixel, unsigned int width, unsigned int height)
{
	if (!std::experimental::filesystem::exists(m_heightmapFilePath))
	{
		std::cerr << "Could not find file: " << m_heightmapFilePath << std::endl;
		return false;
	}

	std::ifstream ifs;
	ifs.open(m_heightmapFilePath, std::ifstream::binary);
	if (ifs.fail())
	{
		std::cerr << "Failed to open file: " << m_heightmapFilePath << std::endl;
		return false;
	}

	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = (bytesPerPixel * width * height);
	const unsigned int fileSize = GetFileLength(ifs);

	if (expectedFileSize != fileSize)
	{
		std::cerr << "Expected file size [" << expectedFileSize << " bytes] differs from actual file size [" << fileSize << " bytes]" << std::endl;
		return false;
	}

	unsigned char* heightMap = new unsigned char[fileSize];
	ifs.read((char*)heightMap, fileSize);
	if (ifs.fail())
	{
		std::cerr << "An error occurred while reading from the height map file: " << m_heightmapFilePath << std::endl;
		ifs.close();
		delete[] heightMap;
		return false;
	}
	ifs.close();

	unsigned int numVerts = width * height;
	m_PositionBuffer.resize(numVerts);
	m_ColorBuffer.resize(numVerts);
	m_NormalBuffer.resize(numVerts);
	m_Tex0Buffer.resize(numVerts);

	m_HeightmapDimensions = glm::vec2(width, height);

	// Size of the terrain in world units
	float terrainWidth = (width - 1) * m_fBlockScale;
	float terrainHeight = (height - 1) * m_fBlockScale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned i = 0; i < width; ++i)
		{
			unsigned int index = (j * width) + i;
			assert(index * bytesPerPixel < fileSize);
			float heightValue = GetHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel);

			float S = (i / (float)(width - 1));
			float T = (j / (float)(height - 1));

			float X = (S * terrainWidth) - halfTerrainWidth;
			float Y = heightValue * m_fHeightScale;
			float Z = (T * terrainHeight) - halfTerrainHeight;

			// Blend 3 textures depending on the height of the terrain
			float tex0Contribution = 0.5f - GetPercentage(heightValue, 0.0f, 0.75f);
			float tex2Contribution = 0.5f - GetPercentage(heightValue, 0.75f, 0.5f);

			m_NormalBuffer[index] = glm::vec3(0);
			m_PositionBuffer[index] = glm::vec3(X, Y, Z);
#if ENABLE_MULTITEXTURE
			m_ColorBuffer[index] = glm::vec4(tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution);
#else
			m_ColorBuffer[index] = glm::vec4(0.5f);
#endif
			m_Tex0Buffer[index] = glm::vec2(S, T);
		}
	}

	std::cout << "Terrain has been loaded!" << std::endl;
	delete[] heightMap;

	GenerateIndexBuffer();
	GenerateNormals();
	GenerateVertexBuffers();

	return true;
}

void Terrain::GenerateIndexBuffer()
{
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2)
	{
		// Terrain hasn't been loaded, or is of an incorrect size
		return;
	}

	const unsigned int terrainWidth = m_HeightmapDimensions.x;
	const unsigned int terrainHeight = m_HeightmapDimensions.y;

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh
	m_IndexBuffer.resize(numTriangles * 3);

	unsigned int index = 0; // Index in the index buffer
	for (unsigned int j = 0; j < (terrainHeight - 1); ++j)
	{
		for (unsigned int i = 0; i < (terrainWidth - 1); ++i)
		{
			int vertexIndex = (j * terrainWidth) + i;
			// Top triangle (T0)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
			m_IndexBuffer[index++] = vertexIndex + 1;                       // V1
																			// Bottom triangle (T1)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
		}
	}
}

void Terrain::GenerateNormals()
{
	for (unsigned int i = 0; i < m_IndexBuffer.size(); i += 3)
	{
		glm::vec3 v0 = m_PositionBuffer[m_IndexBuffer[i + 0]];
		glm::vec3 v1 = m_PositionBuffer[m_IndexBuffer[i + 1]];
		glm::vec3 v2 = m_PositionBuffer[m_IndexBuffer[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		m_NormalBuffer[m_IndexBuffer[i + 0]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 1]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 2]] += normal;
	}

	const glm::vec3 UP(0.0f, 0.5f, 0.0f);
	for (unsigned int i = 0; i < m_NormalBuffer.size(); ++i)
	{
		m_NormalBuffer[i] = glm::normalize(m_NormalBuffer[i]);

#if ENABLE_SLOPE_BASED_BLEND
		float fTexture0Contribution = glm::saturate(glm::dot(m_NormalBuffer[i], UP) - 0.1f);
		m_ColorBuffer[i] = glm::vec4(fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, m_ColorBuffer[i].w);
#endif

	}
}

void Terrain::GenerateVertexBuffers()
{
	// First generate the buffer object ID's
	CreateVertexBuffer(m_GLVertexBuffer);
	CreateVertexBuffer(m_GLNormalBuffer);
	CreateVertexBuffer(m_GLColorBuffer);
	CreateVertexBuffer(m_GLTex0Buffer);
	CreateVertexBuffer(m_GLTex1Buffer);
	CreateVertexBuffer(m_GLTex2Buffer);
	CreateVertexBuffer(m_GLIndexBuffer);

	// Copy the host data into the vertex buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, m_GLVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_PositionBuffer.size(), &(m_PositionBuffer[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_GLColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_ColorBuffer.size(), &(m_ColorBuffer[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_GLNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_NormalBuffer.size(), &(m_NormalBuffer[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_GLTex0Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_GLTex1Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_GLTex2Buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexBuffer.size(), &(m_IndexBuffer[0]), GL_STATIC_DRAW);
}

void Terrain::DeleteTexture(GLuint& texID)
{
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}

float Terrain::GetHeightAt(const glm::vec3& position)
{
	float height = -FLT_MAX;
	// Check if the terrain dimensions are valid
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2) return height;

	// Width and height of the terrain in world units
	float terrainWidth = (m_HeightmapDimensions.x - 1) * m_fBlockScale;
	float terrainHeight = (m_HeightmapDimensions.y - 1) * m_fBlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	glm::vec3 terrainPos = glm::vec3(m_InverseLocalToWorldMatrix * glm::vec4(position, 0.5f));
	glm::vec3 invBlockScale(0.5f / m_fBlockScale, 0.0f, 0.5f / m_fBlockScale);

	// Calculate an offset and scale to get the vertex indices
	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	// Get the 4 vertices that make up the triangle we're over
	glm::vec3 vertexIndices = (terrainPos + offset) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < (int)m_HeightmapDimensions.x && v0 >= 0 && v1 < (int)m_HeightmapDimensions.y)
	{
		glm::vec3 p00 = m_PositionBuffer[(v0 * m_HeightmapDimensions.x) + u0];    // Top-left vertex
		glm::vec3 p10 = m_PositionBuffer[(v0 * m_HeightmapDimensions.x) + u1];    // Top-right vertex
		glm::vec3 p01 = m_PositionBuffer[(v1 * m_HeightmapDimensions.x) + u0];    // Bottom-left vertex
		glm::vec3 p11 = m_PositionBuffer[(v1 * m_HeightmapDimensions.x) + u1];    // Bottom-right vertex

																				  // Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV)
		{   // Top triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else
		{   // Bottom triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		glm::vec3 heightPos = p00 + (dU * percentU) + (dV * percentV);
		// Convert back to world-space by multiplying by the terrain's world matrix
		heightPos = glm::vec3(m_LocalToWorldMatrix * glm::vec4(heightPos, 1));
		height = heightPos.y;
	}

	return height;
}

// Inherited Functions
void Terrain::LoadFromFilePath(std::string filePath)
{
	//m_model->LoadModel(filePath);
	std::ifstream stream(filePath);

	enum class TType
	{
		NONE = -1, HM = 0, TEX = 1
	};

	std::string line;
	std::stringstream ss[2]; // stack allocated array that will store vertex and fragment strings
	TType type = TType::NONE; // set to none (-1) by default
	while (getline(stream, line))
	{
		if (line.find("#terraininfo") != std::string::npos)
		{
			if (line.find("heightmap") != std::string::npos)
				type = TType::HM;
			else if (line.find("texture") != std::string::npos)
				type = TType::TEX;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	// Continue From Here!
	m_heightmapFilePath = ss[0].str();
	m_textureFilePath = ss[1].str();
	// Remove new line characters from string
	m_heightmapFilePath.erase(std::remove(m_heightmapFilePath.begin(), m_heightmapFilePath.end(), '\n'), m_heightmapFilePath.end());
	m_textureFilePath.erase(std::remove(m_textureFilePath.begin(), m_textureFilePath.end(), '\n'), m_textureFilePath.end());
	
	LoadHeightmap(16, 257, 257);
	LoadTexture(0);

}

const void Terrain::Load()
{
	return void();
}

const void Terrain::Unload()
{
	return void();
}

const void Terrain::Prepare(std::string vertShader, std::string fragShader)
{
	return void();
}

const void Terrain::Render()
{
	// Needs to be implemented
	return void();
}

const void Terrain::Destroy()
{
	// Needs to be implemented
	return void();
}

const std::string& Terrain::GetFilePath() const
{
	// Update this
	return "";
}

void Terrain::SetFilePath(const std::string & filePath)
{
	// Update this
	return void();
	//m_filePath = filePath;
}
