/**
* @class TextureManager
* @brief Texture manager class that handles the loading, storing, and managing of all textures
* @author Jack Matters
*
* @date 03/07/2018
* @version 1.0	Initial start. Created the layout of the class, using a singleton and the SOIL API.
*
* @date 07/07/2018
* @version 1.1	Changed from SOIL to SOIL2, as SOIL was deprecated. Got file loading and storing in map
*				working. Next to work on is a better system for loading files (from lua), and using the
*				loaded in textures.
*/

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

// Includes
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "GL\glew.h"
#include "soil2.h"
#include "stb_image.h"

class TextureManager
{
public:
		/**
		* @brief Singleton instance
		*
		* This function returns a singleton instance of the texture manager class so that there is only
		* one instance of a texture manager.
		*
		* @return static TextureManager&
		*/
	static TextureManager& Instance()
	{
		static TextureManager instance;

		return instance;
	}

		/**
		* @brief Activate a texture
		*
		* Sets the given texture to be active
		*
		* @param texID - Texture to set active
		*
		* @return bool - True
		*/
	bool SetActiveTexture(unsigned int texID);

		/**
		* @brief Load a texture
		*
		* Load a texture from file using the SOIL API
		*
		*
		* @return int - texID (given ID of loaded texture)
		*/

		/**
		* @brief Add a texture to the map
		*
		* Adds a texture that is loaded into the map for storing and later access
		*
		* @param filename - Name of file loaded
		* @param texID - ID of texture
		*
		* @return void
		*/
	void AddTextureToMap(std::string filename, int texID);

		/**
		* @brief Release all textures
		*
		* Releases all textures from memory (called in de-constructor
		*
		* @return void
		*/
	void ReleaseAllTextures();

protected:
	/// Width and Height of texture loaded
	int m_width, m_height;

private:

		/**
		* @brief Default constructor
		*
		* This is the default constructor
		*
		* @return null
		*/
	TextureManager();

		/**
		* @brief De-constructor
		*
		* This is the deconstructor
		*/
	~TextureManager();

		/**
		* @brief
		*
		* An unordered map containing all textures for easy lookup
		*/
	std::unordered_map<std::string, unsigned int> m_textureMap;

	/// Number of textures
	int m_numTextures;
};

#endif