/*
* Implementation of TextureManager.h file
* Author - Jack Matters
*/

//Includes
#include "..\headers\TextureManager.h"
#include <sstream>

// Default constructor
TextureManager::TextureManager()
{
	m_numTextures = 0;
}

// De-constructor
TextureManager::~TextureManager()
{
	//ReleaseAllTextures();
}

// Set texture to be active
bool TextureManager::SetActiveTexture(unsigned int texID)
{
	// Enable and bind the passed in texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	return true;
}

// Load texture from file
int TextureManager::LoadTexture(std::string filename)
{
	// Get string as a const char *
	const char* file = filename.c_str();

	// Load in the file
	GLuint newTex = SOIL_load_OGL_texture
	(
		file,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	);

	// Check for error in loading file
	if (newTex == 0)
	{
		std::cout << "SOIL loading error: " << filename << " - " << SOIL_last_result() << std::endl;
		return -1;
	}

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, newTex);

	// Add new texture to map
	AddTextureToMap(filename, newTex);

	/******************** from lab 4 *******************/
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture->getData());
	//glBindTexture(GL_TEXTURE_2D, newTex);
	/***************************************************/

	// Increment number of textures and return ID
	m_numTextures++;
	return newTex;
}

// Add loaded texture to map
void TextureManager::AddTextureToMap(std::string filename, int texID)
{
	// If loaded texture is not already in map, load it
	if (!m_textureMap[filename])
	{
		m_textureMap[filename] = texID;
	}
}

// Return a texture ID
int TextureManager::GetTextureID(std::string filename)
{
	// Get iterator to start of map
	std::unordered_map<std::string, unsigned int>::iterator it = m_textureMap.begin();

	// Search map for texture
	while (it != m_textureMap.end())
	{
		// If texture found, return the given ID
		if ((*it).first == filename)
		{
			return (*it).second;
		}

		// Increment iterator
		it++;
	}

	// If texture is not in map, load it in and add to map
	int texID = LoadTexture(filename);

	// Return texture ID
	return texID;
}

// Release all textures from memory
void TextureManager::ReleaseAllTextures()
{
	// Get iterator to start of map
	std::unordered_map<std::string, unsigned int>::iterator it = m_textureMap.begin();

	// Loop through map, unloading all textures from map
	while (it != m_textureMap.end())
	{
		std::cout << "unloading Texture: " << (*it).first << ", ID: " << (*it).second;
		glDeleteTextures(1, &(*it).second);
		std::cout << " ... done" << std::endl;
		++it;
	}

	// Clear the map
	m_textureMap.clear();
}