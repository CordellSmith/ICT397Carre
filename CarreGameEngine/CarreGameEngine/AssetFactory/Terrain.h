#pragma once

#include "IGameAsset.h"
#include "NewModel.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <string>

#define GLM_SWIZZLE_XYZW
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/std_based_type.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <soil2.h>

	/*
	* @class Terrain
	* @brief Terrain asset
	*
	* This class is used to store all the data about an Terrain. It inherits from
	* the base class IGameAsset and allows for initialisation and drawing of
	* the Terrain.
	*
	* @note Currently not entirely implemented.
	*
	* @author Cordell Smith
	* @version 01
	* @date 29/03/2018 CS
	*
	*/
class Terrain : public IGameAsset
{
public:
		/**
		* @brief Default constructor
		*
		* Empty default constructor.
		*
		* @param float heightScale = 500.0f
		* @param float blockScale = 2.0f
		* @return null
		*/
	Terrain(std::string assetName, float heightScale = 500.0f, float blockScale = 2.0f);

		/**
		* @brief Destructor
		*
		* Empty destructor.
		*
		* @return null
		*/
	~Terrain() { }

		/**
		* @brief Parameter constructor
		*
		* 
		*
		* @param std::string textureFilePath
		* @param std::string heightmapFilePath
		* @return null
		*/
	Terrain(std::string textureFilePath, std::string heightmapFilePath);

	bool LoadTexture(unsigned int textureNo);
	bool LoadHeightmap(unsigned char bitsPerPixel, unsigned int width, unsigned int height);
	void GenerateIndexBuffer();
	void GenerateNormals();
	void GenerateVertexBuffers();
	float GetHeightAt(const glm::vec3& position);

	void DeleteTexture(GLuint& texID);

		/**
		* @brief Loads a Terrain from file
		*
		* Takes the parameter string file path and loads the Terrain data into the
		* corresponding model member variable.
		*
		* @param std::string filePath
		* @return void
		*/
	virtual void LoadFromFilePath(std::string filePath);

		/**
		* @brief Loads
		*
		* Loads the Terrain.
		*
		* @return void
		*/
	virtual const void Load();

		/**
		* @brief Unloads
		*
		* Unloads the Terrain.
		*
		* @return void
		*/
	virtual const void Unload();

	virtual const void Prepare(std::string vertShader, std::string fragShader);

		/**
		* @brief Renders
		*
		* Renders the Terrain.
		*
		* @return void
		*/
	virtual const void Render();

		/**
		* @brief Destroys
		*
		* Destroys the terrain to free up memory.
		*
		* @return void
		*/
	virtual const void Destroy();
	
		/**
		* @brief Gets the file path
		*
		* Returns the file path containing the object information.
		*
		* @return std::string
		*/
	virtual const std::string & GetFilePath() const;

		/**
		* @brief Sets the file path
		*
		* Sets the file path containing the Terrain information.
		*
		* @param std::string& filePath
		* @return void
		*/
	virtual void SetFilePath(const std::string& filePath);

	virtual void SetObjectPosition(glm::vec3 position) { m_model->SetPosition(position); }

	virtual void SetCamera(Camera* camera) { m_camera = camera; }
		
protected:
	/// Stores the file path containing the data
	std::string m_textureFilePath, m_heightmapFilePath;
	/// Model object
	NewModel* m_model;
	/// Camera object
	Camera* m_camera;

	std::vector<glm::vec3> m_PositionBuffer;
	std::vector<glm::vec4> m_ColorBuffer;
	std::vector<glm::vec3> m_NormalBuffer;
	std::vector<glm::vec2> m_Tex0Buffer;
	std::vector<unsigned int> m_IndexBuffer;

	// ID's for the VBO's
	GLuint m_GLVertexBuffer;
	GLuint m_GLNormalBuffer;
	GLuint m_GLColorBuffer;
	GLuint m_GLTex0Buffer;
	GLuint m_GLTex1Buffer;
	GLuint m_GLTex2Buffer;
	GLuint m_GLIndexBuffer;

	static const unsigned int m_uiNumTextures = 3;
	GLuint m_GLTextures[m_uiNumTextures];

	glm::mat4x4 m_LocalToWorldMatrix;
	glm::mat4x4 m_InverseLocalToWorldMatrix;

	// The dimensions of the heightmap texture
	glm::vec2 m_HeightmapDimensions;

	// The height-map value will be multiplied by this value
	// before it is assigned to the vertex's Y-coordinate
	float m_fHeightScale;
	// The vertex's X and Z coordinates will be multiplied by this
	// for each step when building the terrain
	float m_fBlockScale;

};

inline float GetHeightValue(const unsigned char* data, unsigned char numBytes)
{
	switch (numBytes)
	{
	case 1:
	{
		return (unsigned char)(data[0]) / (float)0xff;
	}
	break;
	case 2:
	{
		return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
	}
	break;
	case 4:
	{
		return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
	}
	break;
	default:
	{
		assert(false);  // Height field with non standard pixel sizes
	}
	break;
	}

	return 0.0f;
}

inline int GetFileLength(std::istream& file)
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	// Restore the position of the get pointer
	file.seekg(pos);

	return length;
}

inline float GetPercentage(float value, const float min, const float max)
{
	value = glm::clamp(value, min, max);
	return (value - min) / (max - min);
}

inline void DeleteVertexBuffer(GLuint& vboID)
{
	if (vboID != 0)
	{
		glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}

inline void CreateVertexBuffer(GLuint& vboID)
{
	// Make sure we don't loose the reference to the previous VBO if there is one
	DeleteVertexBuffer(vboID);
	glGenBuffersARB(1, &vboID);
}
